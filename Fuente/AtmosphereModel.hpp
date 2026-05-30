

/*
 * Archivo: AtmosphereModel.hpp
 * Papel técnico: contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#pragma once
#include "TextUtils.hpp"

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · AtmosphereModel.hpp:15 · bloque 1
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {


/*
 * Tipo AtmosphereFoundationOptions: agrupa estado o registros usados por contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct AtmosphereFoundationOptions {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path source_root;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path output_root;
    bool resume = true;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int max_files = 400;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int sample_lines_per_file = 20;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · AtmosphereModel.hpp:58 · bloque 2
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    int live_flush_every_files = 1;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int year_start = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int year_end = 0;
    std::string forced_domain;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · AtmosphereModel.hpp:72 · bloque 3
 * Este tramo pertenece a contrato del modelo atmosférico desacoplado de la interfaz, con tipos persistibles y consultas analíticas de calidad ambiental; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string forced_provider;
    bool inventory_only = false;
/*
 * Declaración void: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · AtmosphereModel.hpp:83 · bloque 4
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    std::function<void(int files_done, int rows_written, const std::string& phase)> progress_callback;
};


/*
 * Tipo AtmosphereFoundationReport: agrupa estado o registros usados por contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct AtmosphereFoundationReport {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int discovered_files = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int indexed_files = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales.
 */
    int skipped_by_checkpoint = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int sampled_files = 0;
    int cloud_points = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · AtmosphereModel.hpp:127 · bloque 5
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    fs::path manifest_jsonl;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · AtmosphereModel.hpp:133 · bloque 6
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    fs::path stac_items_jsonl;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · AtmosphereModel.hpp:139 · bloque 7
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    fs::path model_base_json;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · AtmosphereModel.hpp:145 · bloque 8
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    fs::path cloud_jsonl;
};


/*
 * Tipo AtmosphericReconstructionEngine: agrupa estado o registros usados por contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
class AtmosphericReconstructionEngine {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · AtmosphereModel.hpp:164 · bloque 9
 * Este tramo pertenece a contrato del modelo atmosférico desacoplado de la interfaz, con tipos persistibles y consultas analíticas de calidad ambiental; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
public:
/*
 * Declaración prepare_foundation: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    [[nodiscard]] static AtmosphereFoundationReport prepare_foundation(const AtmosphereFoundationOptions& options);

private:
/*
 * Tipo SourceFile: agrupa estado o registros usados por contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
    struct SourceFile {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        fs::path path;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        std::string file_id;
        std::string kind;
        std::string provider;
        std::string temporal_hint;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales.
 */
        std::string variable_hint;
        std::string delimiter;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
        std::vector<std::string> columns;
        uintmax_t bytes = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        long long mtime_tick = 0;
    };

/*
 * Declaración discover_sources: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · AtmosphereModel.hpp:228 · bloque 10
 * Este tramo pertenece a contrato del modelo atmosférico desacoplado de la interfaz, con tipos persistibles y consultas analíticas de calidad ambiental; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    [[nodiscard]] static std::vector<SourceFile> discover_sources(const AtmosphereFoundationOptions& options);
/*
 * Declaración describe_source: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    [[nodiscard]] static SourceFile describe_source(const fs::path& path, const AtmosphereFoundationOptions& options);
/*
 * Declaración load_checkpoint_ids: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    [[nodiscard]] static std::set<std::string> load_checkpoint_ids(const fs::path& checkpoint);
/*
 * Declaración append_checkpoint: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    static void append_checkpoint(const fs::path& checkpoint, const SourceFile& source);
/*
 * Declaración write_model_base: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    static void write_model_base(const fs::path& out);
/*
 * Declaración append_manifest_rows: expone una operación de contrato público de reconstrucción atmosférica: tipos, opciones, métricas y funciones que separan el procesamiento de datos ambientales del runtime gráfico.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato público del modelo atmosférico, que debe separar tipos persistibles, consultas de agregación y parámetros de calidad sin introducir dependencias gráficas accidentales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    static void append_manifest_rows(const AtmosphereFoundationOptions& options, const std::vector<SourceFile>& rows);
};

}
