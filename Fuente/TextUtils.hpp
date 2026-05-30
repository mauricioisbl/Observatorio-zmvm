

/*
 * Archivo: TextUtils.hpp
 * Papel técnico: contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#pragma once
#include "EpiTypes.hpp"

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TextUtils.hpp:15 · bloque 1
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Declaración trim: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TextUtils.hpp:21 · bloque 2
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::string trim(std::string s);
/*
 * Declaración lower_ascii: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TextUtils.hpp:26 · bloque 3
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::string lower_ascii(std::string s);
/*
 * Declaración clean_user_path_string: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string clean_user_path_string(std::string s);
/*
 * Declaración clean_user_path: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path clean_user_path(const fs::path& p);
/*
 * Declaración resolve_existing_path_relaxed: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path resolve_existing_path_relaxed(const fs::path& p);
/*
 * Declaración strip_accents_utf8: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string strip_accents_utf8(std::string s);
/*
 * Declaración normalize_key: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TextUtils.hpp:51 · bloque 4
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::string normalize_key(std::string s);
/*
 * Declaración html_unescape: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string html_unescape(std::string s);
/*
 * Declaración json_escape: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TextUtils.hpp:61 · bloque 5
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::string json_escape(const std::string& s);
/*
 * Declaración csv_escape: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TextUtils.hpp:66 · bloque 6
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
[[nodiscard]] std::string csv_escape(const std::string& s);
/*
 * Declaración contains_norm: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TextUtils.hpp:71 · bloque 7
 * Este tramo pertenece a contrato de utilidades textuales usadas por importadores, mapas y persistencia; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
[[nodiscard]] bool contains_norm(const std::string& haystack_norm, const std::string& needle_norm);
/*
 * Declaración is_numeric_token: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] bool is_numeric_token(const std::string& s);
/*
 * Declaración parse_epi_int: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TextUtils.hpp:81 · bloque 8
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::optional<int64_t> parse_epi_int(const std::string& s);
/*
 * Declaración safe_filename: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string safe_filename(std::string s);
/*
 * Declaración simple_hash_hex: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string simple_hash_hex(const std::string& s);
/*
 * Declaración read_text_file: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string read_text_file(const fs::path& p);
/*
 * Declaración write_text_file: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
void write_text_file(const fs::path& p, const std::string& content);
/*
 * Declaración ensure_dir: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la API de utilidades textuales, que debe seguir siendo pura, reutilizable y libre de estado gráfico o dependencias de plataforma, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void ensure_dir(const fs::path& p);
/*
 * Declaración file_size_or_zero: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la API de utilidades textuales, que debe seguir siendo pura, reutilizable y libre de estado gráfico o dependencias de plataforma, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · TextUtils.hpp:121 · bloque 9
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
[[nodiscard]] uintmax_t file_size_or_zero(const fs::path& p);
/*
 * Declaración copy_file_overwrite: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · TextUtils.hpp:126 · bloque 10
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
bool copy_file_overwrite(const fs::path& source, const fs::path& destination, std::error_code& ec);
/*
 * Declaración list_pdfs_recursive: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la API de utilidades textuales, que debe seguir siendo pura, reutilizable y libre de estado gráfico o dependencias de plataforma, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TextUtils.hpp:136 · bloque 11
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
[[nodiscard]] std::vector<fs::path> list_pdfs_recursive(const fs::path& root);
/*
 * Declaración now_utc_iso: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string now_utc_iso();
/*
 * Declaración widen_utf8: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::wstring widen_utf8(const std::string& s);
/*
 * Declaración narrow_utf8: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string narrow_utf8(const std::wstring& s);
/*
 * Declaración path_utf8: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TextUtils.hpp:156 · bloque 12
 * Este tramo pertenece a contrato de utilidades textuales usadas por importadores, mapas y persistencia; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
[[nodiscard]] std::string path_utf8(const fs::path& p);
/*
 * Declaración executable_dir: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path executable_dir();
/*
 * Declaración project_root: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path project_root();
/*
 * Declaración config_root: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path config_root();
/*
 * Declaración internal_data_root: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path internal_data_root();
/*
 * Declaración external_data_root: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · TextUtils.hpp:181 · bloque 13
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
[[nodiscard]] fs::path external_data_root();


/*
 * Declaración getenv_utf8_or_empty: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string getenv_utf8_or_empty(const char* name);
/*
 * Declaración getenv_path_utf8: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] fs::path getenv_path_utf8(const char* name);
/*
 * Declaración getenv_wstring_or_empty: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::wstring getenv_wstring_or_empty(const wchar_t* name);


/*
 * Tipo Logger: agrupa estado o registros usados por contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
class Logger {
public:
/*
 * Declaración Logger: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la API de utilidades textuales, que debe seguir siendo pura, reutilizable y libre de estado gráfico o dependencias de plataforma, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    explicit Logger(fs::path log_file);
/*
 * Declaración info: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    void info(const std::string& m);
/*
 * Declaración warn: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    void warn(const std::string& m);
/*
 * Declaración error: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TextUtils.hpp:231 · bloque 14
 * Este tramo pertenece a contrato de utilidades textuales usadas por importadores, mapas y persistencia; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    void error(const std::string& m);
private:
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · TextUtils.hpp:238 · bloque 15
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    std::mutex mu_;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la API de utilidades textuales, que debe seguir siendo pura, reutilizable y libre de estado gráfico o dependencias de plataforma, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · TextUtils.hpp:244 · bloque 16
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ofstream out_;
/*
 * Declaración line: expone una operación de contrato de normalización textual común para evitar divergencias entre parsers, catálogos y UI.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    void line(const std::string& level, const std::string& m);
};

}
