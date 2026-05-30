#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincodec.h>
#include <objbase.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <algorithm>
#include <cstddef>
#include <array>
#include <cstdint>
#include <cwctype>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

template <class T>
void safe_release(T*& p) noexcept {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

class ComApartment {
public:
    ComApartment() noexcept {
        const HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        initialized_ = (hr == S_OK || hr == S_FALSE);
    }
    ~ComApartment() noexcept {
        if (initialized_) CoUninitialize();
    }
    ComApartment(const ComApartment&) = delete;
    ComApartment& operator=(const ComApartment&) = delete;
private:
    bool initialized_ = false;
};

int luminance_bgra(const unsigned char* p) noexcept {
    const int b = p[0];
    const int g = p[1];
    const int r = p[2];
    return (77 * r + 150 * g + 29 * b + 128) >> 8;
}

bool is_already_binary_white_transparent(const std::vector<unsigned char>& px) noexcept {
    if (px.empty() || (px.size() % 4u) != 0u) return false;
    for (size_t i = 0; i < px.size(); i += 4u) {
        const unsigned char b = px[i + 0u];
        const unsigned char g = px[i + 1u];
        const unsigned char r = px[i + 2u];
        const unsigned char a = px[i + 3u];
        if (a == 0u) continue;
        if (a == 255u && r == 255u && g == 255u && b == 255u) continue;
        return false;
    }
    return true;
}

struct BinaryBounds {
    UINT x0 = 0;
    UINT y0 = 0;
    UINT x1 = 0;
    UINT y1 = 0;
    bool any = false;
};

bool is_square_with_icon_tolerance(UINT w, UINT h) noexcept {
    /*
     * TLALPOWA_ICON_POLARIZER_SOLO_PNG_CUADRADOS_2026_05_29:
     * El lote de iconos de Tlalpowa se trata como un conjunto de láminas glíficas,
     * no como un convertidor general de imágenes. Por eso sólo se normalizan PNGs
     * que ya sean esencialmente cuadrados: se tolera una diferencia máxima de cinco
     * píxeles entre ancho y alto para absorber exportaciones imprecisas, pero se
     * rechazan banners, fotografías, miniaturas apaisadas, SVG y cualquier otro
     * recurso cuyo aspecto no corresponda a un glifo centrable. Esta prueba se hace
     * leyendo las dimensiones reales del PNG, no por nombre de archivo, para no
     * depender de bitácoras ni marcas externas y para evitar bucles de reprocesado.
     */
    const int dw = static_cast<int>(w > h ? w - h : h - w);
    return w > 0u && h > 0u && dw <= 5;
}

BinaryBounds find_binary_visible_bounds_bgra(const std::vector<unsigned char>& px, UINT w, UINT h) noexcept {
    BinaryBounds b{};
    if (w == 0u || h == 0u || px.size() != static_cast<size_t>(w) * static_cast<size_t>(h) * 4u) return b;
    b.x0 = w;
    b.y0 = h;
    for (UINT y = 0; y < h; ++y) {
        for (UINT x = 0; x < w; ++x) {
            const size_t i = (static_cast<size_t>(y) * static_cast<size_t>(w) + static_cast<size_t>(x)) * 4u;
            if (px[i + 3u] == 0u) continue;
            b.any = true;
            if (x < b.x0) b.x0 = x;
            if (y < b.y0) b.y0 = y;
            if (x + 1u > b.x1) b.x1 = x + 1u;
            if (y + 1u > b.y1) b.y1 = y + 1u;
        }
    }
    if (!b.any) { b.x0 = b.y0 = b.x1 = b.y1 = 0u; }
    return b;
}

int binary_neighbor_count_bgra(const std::vector<unsigned char>& px, UINT w, UINT h, UINT x, UINT y) noexcept {
    int n = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            const int xx = static_cast<int>(x) + dx;
            const int yy = static_cast<int>(y) + dy;
            if (xx < 0 || yy < 0 || xx >= static_cast<int>(w) || yy >= static_cast<int>(h)) continue;
            const size_t i = (static_cast<size_t>(yy) * static_cast<size_t>(w) + static_cast<size_t>(xx)) * 4u + 3u;
            if (px[i] != 0u) ++n;
        }
    }
    return n;
}

bool refine_binary_edges_once_bgra(std::vector<unsigned char>& px, UINT w, UINT h) noexcept {
    /*
     * TLALPOWA_ICON_POLARIZER_REFINE_EDGES_BINARIO_SUAVE:
     * La refinación de borde debe ser deliberadamente mínima: no antialiasa, no
     * introduce gris, no redibuja el glifo y no cambia su proporción. Sólo elimina
     * ruido blanco completamente aislado y rellena microagujeros rodeados casi por
     * completo por blanco. Con esto se limpian exportaciones raster con motas o
     * píxeles sueltos sin amputar dientes, plumas, volutas ni rasgos estrechos que
     * sí formen parte de la silueta. El resultado sigue siendo exactamente binario:
     * blanco puro opaco o transparencia absoluta.
     */
    if (w < 3u || h < 3u || px.size() != static_cast<size_t>(w) * static_cast<size_t>(h) * 4u) return false;
    const std::vector<unsigned char> old = px;
    bool changed = false;
    for (UINT y = 1u; y + 1u < h; ++y) {
        for (UINT x = 1u; x + 1u < w; ++x) {
            const size_t i = (static_cast<size_t>(y) * static_cast<size_t>(w) + static_cast<size_t>(x)) * 4u;
            const bool on = old[i + 3u] != 0u;
            const int neighbours = binary_neighbor_count_bgra(old, w, h, x, y);
            bool next = on;
            if (on && neighbours == 0) next = false;
            else if (!on && neighbours >= 7) next = true;
            const unsigned char v = next ? 255u : 0u;
            changed = changed || px[i + 0u] != v || px[i + 1u] != v || px[i + 2u] != v || px[i + 3u] != v;
            px[i + 0u] = v;
            px[i + 1u] = v;
            px[i + 2u] = v;
            px[i + 3u] = v;
        }
    }
    return changed;
}

int otsu_threshold_for_visible_pixels_bgra(const std::vector<unsigned char>& px) noexcept {
    std::array<unsigned long long, 256> hist{};
    unsigned long long total = 0;
    unsigned long long sum = 0;
    for (size_t i = 0; i + 3u < px.size(); i += 4u) {
        const unsigned char a = px[i + 3u];
        if (a < 16u) continue;
        const int lum = luminance_bgra(&px[i]);
        ++hist[static_cast<size_t>(lum)];
        ++total;
        sum += static_cast<unsigned long long>(lum);
    }
    if (total == 0u) return 196;

    unsigned long long weight_b = 0;
    unsigned long long sum_b = 0;
    double best_score = -1.0;
    int best_t = 196;
    for (int t = 0; t < 256; ++t) {
        weight_b += hist[static_cast<size_t>(t)];
        if (weight_b == 0u) continue;
        const unsigned long long weight_f = total - weight_b;
        if (weight_f == 0u) break;
        sum_b += static_cast<unsigned long long>(t) * hist[static_cast<size_t>(t)];
        const double mean_b = static_cast<double>(sum_b) / static_cast<double>(weight_b);
        const double mean_f = static_cast<double>(sum - sum_b) / static_cast<double>(weight_f);
        const double d = mean_b - mean_f;
        const double score = static_cast<double>(weight_b) * static_cast<double>(weight_f) * d * d;
        if (score > best_score) {
            best_score = score;
            best_t = t;
        }
    }

    /*
     * TLALPOWA_ICON_POLARIZER_THRESHOLD_2026_05_29:
     * El umbral estadístico se restringe de forma deliberada. Los íconos de
     * poblado provienen de glifos blancos sobre fondos negros o de versiones ya
     * parcialmente limpiadas; si Otsu cae demasiado bajo por predominio masivo de
     * negro, cualquier ruido gris tenue podría sobrevivir, y si sube demasiado por
     * miniaturas casi vacías se amputarían rasgos finos. Este rango fuerza una
     * polarización agresiva: sólo la zona verdaderamente clara se convierte en
     * blanco sólido, todo lo demás queda transparente. No existe una tercera clase
     * visual, no se conserva antialias gris y no se escribe bitácora para decidir
     * si la imagen fue procesada; la detección se hace leyendo los píxeles reales.
     */
    return std::clamp(best_t, 112, 224);
}

bool polarize_in_memory_bgra(std::vector<unsigned char>& px) noexcept {
    if (is_already_binary_white_transparent(px)) return false;
    const int threshold = otsu_threshold_for_visible_pixels_bgra(px);
    bool changed = false;
    bool any_white = false;
    for (size_t i = 0; i + 3u < px.size(); i += 4u) {
        const unsigned char old_b = px[i + 0u];
        const unsigned char old_g = px[i + 1u];
        const unsigned char old_r = px[i + 2u];
        const unsigned char old_a = px[i + 3u];
        const bool keep = old_a >= 16u && luminance_bgra(&px[i]) >= threshold;
        const unsigned char nb = keep ? 255u : 0u;
        const unsigned char ng = keep ? 255u : 0u;
        const unsigned char nr = keep ? 255u : 0u;
        const unsigned char na = keep ? 255u : 0u;
        any_white = any_white || keep;
        changed = changed || old_b != nb || old_g != ng || old_r != nr || old_a != na;
        px[i + 0u] = nb;
        px[i + 1u] = ng;
        px[i + 2u] = nr;
        px[i + 3u] = na;
    }
    return changed && any_white;
}

bool decode_png_bgra(IWICImagingFactory* factory, const fs::path& path, UINT& w, UINT& h, std::vector<unsigned char>& px) noexcept {
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;
    bool ok = false;

    HRESULT hr = factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ,
                                                    WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) goto done;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) goto done;
    hr = frame->GetSize(&w, &h);
    if (FAILED(hr) || w == 0u || h == 0u || w > 8192u || h > 8192u) goto done;
    if (static_cast<unsigned long long>(w) * static_cast<unsigned long long>(h) > 16777216ull) goto done;

    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) goto done;
    hr = converter->Initialize(frame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone,
                               nullptr, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) goto done;

    px.assign(static_cast<size_t>(w) * static_cast<size_t>(h) * 4u, 0u);
    hr = converter->CopyPixels(nullptr, w * 4u, static_cast<UINT>(px.size()), px.data());
    if (FAILED(hr)) goto done;
    ok = true;

done:
    safe_release(converter);
    safe_release(frame);
    safe_release(decoder);
    return ok;
}

bool encode_png_bgra_atomic(IWICImagingFactory* factory, const fs::path& path, UINT w, UINT h, const std::vector<unsigned char>& px) noexcept {
    if (w == 0u || h == 0u || px.size() != static_cast<size_t>(w) * static_cast<size_t>(h) * 4u) return false;
    const fs::path tmp = fs::path(path.wstring() + L".tlalpowa.tmp.png");
    std::error_code ec;
    fs::remove(tmp, ec);

    IWICStream* stream = nullptr;
    IWICBitmapEncoder* encoder = nullptr;
    IWICBitmapFrameEncode* frame = nullptr;
    IPropertyBag2* bag = nullptr;
    bool ok = false;

    HRESULT hr = factory->CreateStream(&stream);
    if (FAILED(hr)) goto done;
    hr = stream->InitializeFromFilename(tmp.c_str(), GENERIC_WRITE);
    if (FAILED(hr)) goto done;
    hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
    if (FAILED(hr)) goto done;
    hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
    if (FAILED(hr)) goto done;
    hr = encoder->CreateNewFrame(&frame, &bag);
    if (FAILED(hr)) goto done;
    hr = frame->Initialize(bag);
    if (FAILED(hr)) goto done;
    hr = frame->SetSize(w, h);
    if (FAILED(hr)) goto done;
    {
        WICPixelFormatGUID fmt = GUID_WICPixelFormat32bppBGRA;
        hr = frame->SetPixelFormat(&fmt);
        if (FAILED(hr) || !IsEqualGUID(fmt, GUID_WICPixelFormat32bppBGRA)) goto done;
    }
    hr = frame->WritePixels(h, w * 4u, static_cast<UINT>(px.size()), const_cast<unsigned char*>(px.data()));
    if (FAILED(hr)) goto done;
    hr = frame->Commit();
    if (FAILED(hr)) goto done;
    hr = encoder->Commit();
    if (FAILED(hr)) goto done;

    safe_release(bag);
    safe_release(frame);
    safe_release(encoder);
    safe_release(stream);

    ok = MoveFileExW(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
    if (!ok) fs::remove(tmp, ec);
    return ok;

done:
    safe_release(bag);
    safe_release(frame);
    safe_release(encoder);
    safe_release(stream);
    fs::remove(tmp, ec);
    return false;
}

bool normalize_square_icon_to_2048_bgra(UINT& w, UINT& h, std::vector<unsigned char>& px) noexcept {
    /*
     * TLALPOWA_ICON_POLARIZER_NORMALIZACION_CANONICA_2048:
     * Contrato completo del tratamiento de iconos en compilación:
     *   1) sólo entra PNG casi cuadrado, con tolerancia de cinco píxeles;
     *   2) se polariza a blanco puro/transparente, eliminando negro, gris y alfa intermedio;
     *   3) se recorta el rectángulo mínimo que contiene píxeles blancos;
     *   4) el recorte se centra en un lienzo cuadrado, sin deformar el glifo;
     *   5) el lienzo cuadrado se normaliza a 2048 x 2048 por vecino más cercano;
     *   6) se aplica una limpieza binaria mínima de motas/agujeros, sin antialias;
     *   7) se compara con el archivo leído: si ya era exactamente ese resultado, no se reescribe.
     *
     * Esta secuencia impide bucles: no se usa manifiesto, log ni marca externa. La
     * idempotencia se verifica contra los píxeles reales. Un icono ya limpio vuelve
     * a producir la misma matriz 2048x2048 y por tanto no toca el disco. Los SVG no
     * llegan aquí porque el escaneo filtra por extensión .png antes de decodificar.
     */
    if (!is_square_with_icon_tolerance(w, h)) return false;
    if (px.size() != static_cast<size_t>(w) * static_cast<size_t>(h) * 4u) return false;

    const UINT original_w = w;
    const UINT original_h = h;
    const std::vector<unsigned char> original = px;

    const bool had_non_binary = !is_already_binary_white_transparent(px);
    (void)polarize_in_memory_bgra(px);
    (void)refine_binary_edges_once_bgra(px, w, h);

    BinaryBounds bb = find_binary_visible_bounds_bgra(px, w, h);
    if (!bb.any || bb.x1 <= bb.x0 || bb.y1 <= bb.y0) return false;

    const UINT crop_w = bb.x1 - bb.x0;
    const UINT crop_h = bb.y1 - bb.y0;
    if (crop_w == 0u || crop_h == 0u || crop_w > 8192u || crop_h > 8192u) return false;

    std::vector<unsigned char> crop(static_cast<size_t>(crop_w) * static_cast<size_t>(crop_h) * 4u, 0u);
    for (UINT y = 0; y < crop_h; ++y) {
        const size_t src = (static_cast<size_t>(bb.y0 + y) * static_cast<size_t>(w) + static_cast<size_t>(bb.x0)) * 4u;
        const size_t dst = static_cast<size_t>(y) * static_cast<size_t>(crop_w) * 4u;
        std::copy(px.begin() + static_cast<std::ptrdiff_t>(src),
                  px.begin() + static_cast<std::ptrdiff_t>(src + static_cast<size_t>(crop_w) * 4u),
                  crop.begin() + static_cast<std::ptrdiff_t>(dst));
    }
    (void)refine_binary_edges_once_bgra(crop, crop_w, crop_h);

    const UINT square_side = (crop_w >= crop_h) ? crop_w : crop_h;
    if (square_side == 0u) return false;
    constexpr UINT kCanonicalSide = 2048u;
    const UINT offset_x = (square_side - crop_w) / 2u;
    const UINT offset_y = (square_side - crop_h) / 2u;

    std::vector<unsigned char> normalized(static_cast<size_t>(kCanonicalSide) * static_cast<size_t>(kCanonicalSide) * 4u, 0u);
    for (UINT y = 0; y < kCanonicalSide; ++y) {
        const UINT src_square_y = static_cast<UINT>((static_cast<unsigned long long>(y) * static_cast<unsigned long long>(square_side)) / static_cast<unsigned long long>(kCanonicalSide));
        if (src_square_y < offset_y || src_square_y >= offset_y + crop_h) continue;
        const UINT cy = src_square_y - offset_y;
        for (UINT x = 0; x < kCanonicalSide; ++x) {
            const UINT src_square_x = static_cast<UINT>((static_cast<unsigned long long>(x) * static_cast<unsigned long long>(square_side)) / static_cast<unsigned long long>(kCanonicalSide));
            if (src_square_x < offset_x || src_square_x >= offset_x + crop_w) continue;
            const UINT cx = src_square_x - offset_x;
            const size_t si = (static_cast<size_t>(cy) * static_cast<size_t>(crop_w) + static_cast<size_t>(cx)) * 4u;
            if (crop[si + 3u] == 0u) continue;
            const size_t di = (static_cast<size_t>(y) * static_cast<size_t>(kCanonicalSide) + static_cast<size_t>(x)) * 4u;
            normalized[di + 0u] = 255u;
            normalized[di + 1u] = 255u;
            normalized[di + 2u] = 255u;
            normalized[di + 3u] = 255u;
        }
    }
    (void)refine_binary_edges_once_bgra(normalized, kCanonicalSide, kCanonicalSide);

    if (original_w == kCanonicalSide && original_h == kCanonicalSide && original == normalized && !had_non_binary) return false;
    w = kCanonicalSide;
    h = kCanonicalSide;
    px.swap(normalized);
    return true;
}

void process_png_file(IWICImagingFactory* factory, const fs::path& path) noexcept {
    UINT w = 0, h = 0;
    std::vector<unsigned char> px;
    if (!decode_png_bgra(factory, path, w, h, px)) return;
    if (!normalize_square_icon_to_2048_bgra(w, h, px)) return;
    (void)encode_png_bgra_atomic(factory, path, w, h, px);
}

bool is_png_path(const fs::path& p) {
    std::wstring ext = p.extension().wstring();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });
    /*
     * TLALPOWA_ICON_POLARIZER_NO_SVG:
     * Los SVG no se rasterizan ni se transforman aquí. Si el usuario decide usar
     * SVG en el futuro, necesitará una ruta explícita de rasterización vectorial;
     * este ejecutable sólo toca PNGs ya rasterizados y casi cuadrados, para no
     * convertir por accidente ilustraciones vectoriales ni alterar fuentes maestras.
     */
    return ext == L".png";
}

void process_icon_directory(IWICImagingFactory* factory, const fs::path& dir) noexcept {
    std::error_code ec;
    if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) return;
    const fs::directory_options opts = fs::directory_options::skip_permission_denied;
    for (fs::recursive_directory_iterator it(dir, opts, ec), end; it != end; it.increment(ec)) {
        if (ec) { ec.clear(); continue; }
        const fs::path p = it->path();
        const std::wstring name = p.filename().wstring();
        if (name.find(L".tlalpowa.tmp") != std::wstring::npos) continue;
        if (!fs::is_regular_file(p, ec) || !is_png_path(p)) continue;
        process_png_file(factory, p);
    }
}

std::vector<fs::path> roots_from_command_line(int argc, wchar_t** argv) {
    std::vector<fs::path> roots;
    for (int i = 1; i < argc; ++i) {
        const std::wstring arg = argv[i] ? argv[i] : L"";
        if (arg == L"--root" && i + 1 < argc) {
            const fs::path base(argv[++i]);
            roots.push_back(base / L"Datos" / L"icon");
            roots.push_back(base / L"Datos" / L"Icon");
            roots.push_back(base / L"Datos" / L"Iconos");
            roots.push_back(base / L"Datos" / L"icons");
        } else if (!arg.empty()) {
            roots.emplace_back(arg);
        }
    }
    return roots;
}

} // namespace

int wmain(int argc, wchar_t** argv) {
    /*
     * TLALPOWA_ICON_POLARIZER_BUILD_STEP_2026_05_29:
     * Este ejecutable no usa un manifiesto, sello temporal, bitácora ni contador
     * para decidir si un glifo ya fue tratado. Cada compilación vuelve a mirar los
     * píxeles y sólo escribe cuando encuentra algo distinto de dos estados exactos:
     * alfa 0 completamente transparente o blanco puro opaco 255/255/255/255. Así
     * no hay bucles de reprocesamiento, no depende de logs que se puedan borrar, y
     * tampoco puede reintroducir gris por redondeos acumulados. El resultado final
     * aceptado es binario: lo blanco sobrevive como blanco mate absoluto y todo lo
     * negro, gris, semitransparente o contaminado desaparece como transparencia.
     */
    ComApartment com;
    IWICImagingFactory* factory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&factory));
    if (FAILED(hr) || !factory) return 0;

    std::vector<fs::path> roots = roots_from_command_line(argc, argv);
    if (roots.empty()) roots.push_back(fs::current_path() / L"Datos" / L"icon");
    for (const fs::path& root : roots) process_icon_directory(factory, root);

    safe_release(factory);
    return 0;
}
