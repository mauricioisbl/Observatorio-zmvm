/*
 * Archivo: TemporalBlocks.hpp
 * Papel técnico: tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#pragma once
#include "TextUtils.hpp"
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · TemporalBlocks.hpp:9 · bloque 1
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
#include "Ixiptlah.hpp"

#include <cstdint>
#include <vector>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TemporalBlocks.hpp:17 · bloque 2
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Tipo TemporalEpidemiologyRecord: agrupa estado o registros usados por tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:28 · bloque 3
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
struct TemporalEpidemiologyRecord {
    std::string entity;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int year = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int epi_week = 0;
    int page = 0;
    std::string disease;
    std::string cie10;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:45 · bloque 4
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string jurisdiction;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
    std::string period;
    std::string sex;
    int64_t value = 0;
};

/*
 * Tipo TemporalAtmospherePackedStation: agrupa estado o registros usados por tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct TemporalAtmospherePackedStation {
    std::string id;
    std::string name;
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
    double lon = 0.0;
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
    double lat = 0.0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
    double alt = 0.0;
};


/*
 * Tipo TemporalAtmospherePackedSample: agrupa estado o registros usados por tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct TemporalAtmospherePackedSample {
    std::uint8_t day = 1;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    std::uint8_t hour = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    std::uint8_t minute = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:112 · bloque 5
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::uint32_t station_index = 0;
    std::uint32_t pollutant_index = 0;
    std::uint32_t unit_index = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
    double value = 0.0;
};

/*
 * Declaración temporal_lustrum_start_for_year: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
[[nodiscard]] int temporal_lustrum_start_for_year(int year);
/*
 * Declaración temporal_lustrum_label_for_year: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string temporal_lustrum_label_for_year(int year);
/*
 * Declaración temporal_iso_date_from_epi_week: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string temporal_iso_date_from_epi_week(int year, int week);
/*
 * Declaración temporal_year_from_date_or_fields: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] int temporal_year_from_date_or_fields(const std::string& date, const std::string& year_hint);
/*
 * Declaración temporal_block_columns: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] const std::vector<std::string>& temporal_block_columns();
/*
 * Declaración temporal_block_header_line: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string temporal_block_header_line();
/*
 * Declaración split_tsv_lossless: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TemporalBlocks.hpp:162 · bloque 6
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::vector<std::string> split_tsv_lossless(const std::string& line);
/*
 * Declaración tsv_escape_field: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::string tsv_escape_field(std::string value);
/*
 * Declaración temporal_is_lustrum_data_file: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] bool temporal_is_lustrum_data_file(const fs::path& p, const std::string& required_extension = {});
/*
 * Declaración temporal_tsv_files: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
[[nodiscard]] std::vector<fs::path> temporal_tsv_files(const fs::path& root);
/*
 * Declaración temporal_csv_files: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:182 · bloque 7
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
[[nodiscard]] std::vector<fs::path> temporal_csv_files(const fs::path& root);
/*
 * Declaración temporal_json_files: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TemporalBlocks.hpp:187 · bloque 8
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::vector<fs::path> temporal_json_files(const fs::path& root);
/*
 * Declaración temporal_ixiptlah_file: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · TemporalBlocks.hpp:192 · bloque 9
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
[[nodiscard]] fs::path temporal_ixiptlah_file(const fs::path& root);
/*
 * Declaración temporal_ixiptlah_files: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · TemporalBlocks.hpp:197 · bloque 10
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
[[nodiscard]] std::vector<fs::path> temporal_ixiptlah_files(const fs::path& root);
/*
 * Declaración temporal_has_ixiptlah_records: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · TemporalBlocks.hpp:202 · bloque 11
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
[[nodiscard]] bool temporal_has_ixiptlah_records(const fs::path& root);
void temporal_read_epidemiology_records(const fs::path& root,
/*
 * Declaración bool: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:213 · bloque 12
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
                                        const std::function<bool(const TemporalEpidemiologyRecord&)>& on_record);
/*
 * Declaración temporal_append_record: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_record(const fs::path& root, const std::map<std::string, std::string>& fields);

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_source_inventory_record(const fs::path& root,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                             const std::map<std::string, std::string>& fields);

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_atmosphere_measurement(const fs::path& root,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                           const std::string& date,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                           const std::string& hour,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                           int year,
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:262 · bloque 13
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                                           const std::string& domain,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
                                           const std::string& source_id,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                           const std::string& source_file,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                           const std::string& source_path,
                                           const std::string& pollutant,
                                           const std::string& station_id,
                                           const std::string& station,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                           double lon,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                           double lat,
                                           double alt,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
                                           const std::string& metric,
                                           double value,
                                           const std::string& unit);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_atmosphere_measurement_batch(const fs::path& root,
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:311 · bloque 14
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                                                  const std::string& source_id,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                                  const std::string& source_file,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                                  const std::string& source_path,
                                                  const std::string& domain,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                                  int year,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TemporalBlocks.hpp:336 · bloque 15
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                                                  int month,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                                  const std::vector<TemporalAtmospherePackedStation>& stations,
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                                                  const std::vector<std::string>& pollutants,
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                                                  const std::vector<std::string>& units,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                                  const std::vector<TemporalAtmospherePackedSample>& samples);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_atmosphere_render_summary(const fs::path& root,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                               const std::string& date,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                               const std::string& hour,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                               int year,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:390 · bloque 16
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                                               const std::string& pollutant,
                                               const std::string& station_id,
                                               const std::string& station,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                               double lon,
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                                               double lat,
                                               double alt,
                                               double mean,
                                               double min_value,
                                               double max_value,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
                                               const std::string& unit,
                                               int64_t count);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void temporal_append_atmosphere_territory_average(const fs::path& root,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                                  const std::string& date,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                                  const std::string& hour,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:439 · bloque 17
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                                                  int year,
                                                  const std::string& entity_code,
                                                  const std::string& territory_code,
                                                  const std::string& territory_name,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
                                                  const std::string& pollutant,
                                                  const std::string& metric,
                                                  double value,
                                                  const std::string& unit,
                                                  int64_t count);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:458 · bloque 18
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void temporal_append_epidemiology_record(const fs::path& root,
                                        const std::string& entity,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                        int year,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
                                        int epi_week,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos.
 */
                                        int page,
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · TemporalBlocks.hpp:478 · bloque 19
 * Este tramo pertenece a contrato temporal compartido entre interfaz, importadores y persistencia mensual; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                                        const std::string& disease,
                                        const std::string& cie10,
                                        const std::string& jurisdiction,
                                        const std::string& period,
                                        const std::string& sex,
                                        int64_t value);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:489 · bloque 20
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void temporal_append_epidemiology_records_batch(const fs::path& root,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:495 · bloque 21
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
                                                const std::vector<TemporalEpidemiologyRecord>& records);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:501 · bloque 22
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void temporal_append_epidemiology_records_batch_exact_root(const fs::path& root,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:507 · bloque 23
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
                                                           const std::vector<TemporalEpidemiologyRecord>& records);
/*
 * Declaración temporal_flush_append_streams: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TemporalBlocks.hpp:512 · bloque 24
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void temporal_flush_append_streams();
/*
 * Declaración temporal_flush_append_streams_if_due: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TemporalBlocks.hpp:517 · bloque 25
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void temporal_flush_append_streams_if_due(int min_interval_ms);
/*
 * Declaración temporal_close_append_streams: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
void temporal_close_append_streams();
/*
 * Declaración temporal_purge_atmosphere_category: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · TemporalBlocks.hpp:532 · bloque 26
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
[[nodiscard]] int temporal_purge_atmosphere_category(const fs::path& root, const std::string& category, int year_start = 0, int year_end = 0);
/*
 * Declaración temporal_purge_epidemiology_records: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · TemporalBlocks.hpp:537 · bloque 27
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
[[nodiscard]] int temporal_purge_epidemiology_records(const fs::path& root, int year_start = 0, int year_end = 0);
/*
 * Declaración temporal_rebuild_json_index_for_tsv: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TemporalBlocks.hpp:542 · bloque 28
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
void temporal_rebuild_json_index_for_tsv(const fs::path& tsv_path);
/*
 * Declaración temporal_rebuild_all_json_indexes: expone una operación de tipos temporales compartidos entre epidemiología, atmósfera, movilidad y renderizado histórico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos temporales compactos usados como frontera estable entre UI, importadores y serialización de intervalos históricos, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · TemporalBlocks.hpp:552 · bloque 29
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
void temporal_rebuild_all_json_indexes(const fs::path& root);

}
