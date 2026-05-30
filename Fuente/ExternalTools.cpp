

/*
 * Archivo: ExternalTools.cpp
 * Papel técnico: adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:9 · bloque 1
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
#include "ExternalTools.hpp"
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <cstdlib>
#include <algorithm>
#include <chrono>
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:20 · bloque 2
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
#include <thread>
#include <mutex>
#include <atomic>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:27 · bloque 3
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:32 · bloque 4
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace {


/*
 * Función executable_exists: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
bool executable_exists(const fs::path& p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p.empty()) return false;
 */
    if (p.empty()) return false;
    std::error_code ec;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fs::is_regular_file(p, ec) && !ec) return true;
 */
    if (fs::is_regular_file(p, ec) && !ec) return true;
#ifdef _WIN32
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const DWORD attr = GetFileAttributesW(p.wstring().c_str());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:71 · bloque 5
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
#else
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return false;
#endif
}


/*
 * Función env_int_clamped: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
int env_int_clamped(const char* name, int fallback, int lo, int hi) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    const std::string raw = getenv_utf8_or_empty(name);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (raw.empty()) return std::clamp(fallback, lo, hi);
 */
    if (raw.empty()) return std::clamp(fallback, lo, hi);
    try { return std::clamp(std::stoi(raw), lo, hi); } catch (...) { return std::clamp(fallback, lo, hi); }
}

/*
 * Función env_flag_enabled: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
bool env_flag_enabled(const char* name) {
    const std::string raw = getenv_utf8_or_empty(name);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (raw.empty()) return false;
 */
    if (raw.empty()) return false;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:127 · bloque 6
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    const std::string v = lower_ascii(trim(raw));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return v == "1" || v == "true" || v == "on" || v == "yes" || v == "si";
}


/*
 * Función poppler_process_budget: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:147 · bloque 7
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
int poppler_process_budget() {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:153 · bloque 8
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    const unsigned hw = std::max(1u, std::thread::hardware_concurrency());
    const int fallback = static_cast<int>(std::clamp<unsigned>(hw <= 4 ? 2u : hw / 2u, 2u, 6u));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:160 · bloque 9
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    return env_int_clamped("TLALPOWA_POPPLER_MAX_PROCESSES", fallback, 1, 16);
}


/*
 * Tipo ExternalProcessSlot: agrupa estado o registros usados por adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:173 · bloque 10
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
class ExternalProcessSlot {
public:
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:180 · bloque 11
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    explicit ExternalProcessSlot(int max_active) : max_active_(std::max(1, max_active)) {
        int observed = active_.load();
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (;;) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:188 · bloque 12
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            observed = active_.load();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (observed < max_active_ && active_.compare_exchange_weak(observed, observed + 1)) break;
 */
            if (observed < max_active_ && active_.compare_exchange_weak(observed, observed + 1)) break;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:200 · bloque 13
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
            std::this_thread::sleep_for(std::chrono::milliseconds(18));
        }
    }
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:203 · bloque 14
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    ~ExternalProcessSlot() { active_.fetch_sub(1); }
    ExternalProcessSlot(const ExternalProcessSlot&) = delete;
    ExternalProcessSlot& operator=(const ExternalProcessSlot&) = delete;
private:
    int max_active_ = 1;
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:213 · bloque 15
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    static std::atomic<int> active_;
};

/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:221 · bloque 16
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
std::atomic<int> ExternalProcessSlot::active_{0};

/*
 * Función preview_render_extension: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
std::string preview_render_extension() {
    const std::string raw = getenv_utf8_or_empty("TLALPOWA_PREVIEW_FORMAT");
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:235 · bloque 17
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    const std::string v = raw.empty() ? std::string{} : lower_ascii(trim(raw));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return v == "png" ? ".png" : ".jpg";
}

/*
 * Función preview_render_jpeg: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
bool preview_render_jpeg() {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return preview_render_extension() == ".jpg";
}


/*
 * Función split_path_env: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:274 · bloque 18
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
std::vector<fs::path> split_path_env() {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::vector<fs::path> out;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const std::string text = getenv_utf8_or_empty("PATH");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (text.empty()) return out;
 */
    if (text.empty()) return out;
#ifdef _WIN32
    const char sep = ';';
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:295 · bloque 19
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
#else
    const char sep = ':';
#endif
    std::stringstream ss(text);
    std::string part;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (std::getline(ss, part, sep)) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:306 · bloque 20
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        part = trim(part);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!part.empty()) out.emplace_back(widen_utf8(part));
 */
        if (!part.empty()) out.emplace_back(widen_utf8(part));
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return out;
}

/*
 * Función system_dependency_roots: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
std::vector<fs::path> system_dependency_roots() {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::vector<fs::path> roots;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    auto add_root = [&](const fs::path& root) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (root.empty()) return;
 */
        if (root.empty()) return;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:356 · bloque 21
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        const fs::path normalized = root.lexically_normal();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:362 · bloque 22
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        const auto key = path_utf8(normalized);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (const auto& existing : roots) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (path_utf8(existing.lexically_normal()) == key) return;
 */
            if (path_utf8(existing.lexically_normal()) == key) return;
        }
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:381 · bloque 23
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        roots.push_back(normalized);
    };


/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    add_root(executable_dir() / "Dependencias");
    const std::string deps_root = getenv_utf8_or_empty("TLALPOWA_DEPS_ROOT");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!deps_root.empty()) {
 */
    if (!deps_root.empty()) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:398 · bloque 24
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        const std::string cleaned = trim(deps_root);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!cleaned.empty()) add_root(fs::path(widen_utf8(cleaned)));
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:404 · bloque 25
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (!cleaned.empty()) add_root(fs::path(widen_utf8(cleaned)));
    }
#ifdef _WIN32
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    add_root(fs::path(L"C:/ProgramData/Tlalpowa/Dependencias"));


/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    add_root(fs::path(L"C:/ProgramData/Observatorio-ZMVM/Dependencias"));
#endif
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return roots;
}

/*
 * Función system_dependency_root: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
fs::path system_dependency_root() {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    const auto roots = system_dependency_roots();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    return roots.empty() ? fs::path{} : roots.front();
}

}


/*
 * Función resolve_tool_path: ingiere, localiza o interpreta datos de entrada y debe fallar de forma contenida ante archivos incompletos, rutas ausentes o formatos parciales.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:468 · bloque 26
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
fs::path ExternalTools::resolve_tool_path(const fs::path& configured, const std::string& exe_name) const {
#ifdef _WIN32
    const std::string bare = exe_name;
#else
    std::string bare = exe_name;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (bare.size() > 4 && lower_ascii(bare.substr(bare.size() - 4)) == ".exe") bare.resize(bare.size() - 4);
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:478 · bloque 27
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    if (bare.size() > 4 && lower_ascii(bare.substr(bare.size() - 4)) == ".exe") bare.resize(bare.size() - 4);
#endif
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::vector<fs::path> candidates;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!configured.empty()) candidates.push_back(configured);
 */
    if (!configured.empty()) candidates.push_back(configured);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!configured.empty() && !configured.has_parent_path()) {
 */
    if (!configured.empty() && !configured.has_parent_path()) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:503 · bloque 28
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        for (const auto& d : split_path_env()) candidates.push_back(d / configured);
    }
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (const auto& deps_root : system_dependency_roots()) {
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        candidates.push_back(deps_root / "poppler" / "Library" / "bin" / exe_name);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:522 · bloque 29
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        candidates.push_back(deps_root / "poppler" / "bin" / exe_name);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        candidates.push_back(deps_root / "bin" / exe_name);
    }
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:535 · bloque 30
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    for (const auto& d : split_path_env()) {
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        candidates.push_back(d / bare);
#ifdef _WIN32
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        candidates.push_back(d / exe_name);
#endif
    }

/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (const auto& c : candidates) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (executable_exists(c)) return c;
 */
        if (executable_exists(c)) return c;
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    return configured.empty() ? fs::path(widen_utf8(bare)) : configured;
}


/*
 * Función validate: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:584 · bloque 31
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
void ExternalTools::validate() const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:590 · bloque 32
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdftotext = resolve_tool_path(options_.pdftotext, "pdftotext.exe");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!executable_exists(pdftotext)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:596 · bloque 33
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (!executable_exists(pdftotext)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:602 · bloque 34
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        throw std::runtime_error("No se encontro pdftotext.exe para extraer boletines PDF. Busque en TLALPOWA_DEPS_ROOT, C:/ProgramData/Tlalpowa/Dependencias, C:/ProgramData/Observatorio-ZMVM/Dependencias y PATH. Ruta intentada: " + path_utf8(pdftotext));
    }


}

/*
 * Función external_work_root: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:618 · bloque 35
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
fs::path ExternalTools::external_work_root() const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:624 · bloque 36
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    return fs::temp_directory_path() / "Tlalpowa" / "external_work";
}


/*
 * Función stage_pdf_ascii: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:638 · bloque 37
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
fs::path ExternalTools::stage_pdf_ascii(const fs::path& pdf, const std::string& purpose) const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:644 · bloque 38
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const std::string id = simple_hash_hex(path_utf8(pdf) + "|" + purpose + "|" + std::to_string(file_size_or_zero(pdf)));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:650 · bloque 39
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const fs::path dir = external_work_root() / safe_filename(id);
    ensure_dir(dir);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:657 · bloque 40
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path staged = dir / "input.pdf";
    std::error_code ec;


/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:666 · bloque 41
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    static std::mutex stage_mu;
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:672 · bloque 42
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    std::lock_guard<std::mutex> lk(stage_mu);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!fs::exists(staged) || file_size_or_zero(staged) != file_size_or_zero(pdf)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:678 · bloque 43
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (!fs::exists(staged) || file_size_or_zero(staged) != file_size_or_zero(pdf)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:684 · bloque 44
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        const fs::path tmp = dir / ("input_" + simple_hash_hex(path_utf8(pdf) + now_utc_iso()) + ".tmp");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:690 · bloque 45
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        copy_file_overwrite(pdf, tmp, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) throw std::runtime_error("No se pudo preparar copia ASCII temporal para Poppler: " + ec.messag...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:696 · bloque 46
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (ec) throw std::runtime_error("No se pudo preparar copia ASCII temporal para Poppler: " + ec.message());
        fs::rename(tmp, staged, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
        if (ec) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:709 · bloque 47
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            copy_file_overwrite(tmp, staged, ec);
            std::error_code rm_ec;
            fs::remove(tmp, rm_ec);
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) throw std::runtime_error("No se pudo publicar copia ASCII temporal para Poppler: " + ec.messag...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:718 · bloque 48
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (ec) throw std::runtime_error("No se pudo publicar copia ASCII temporal para Poppler: " + ec.message());
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return staged;
}


/*
 * Función pdf_page_count: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:739 · bloque 49
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
int ExternalTools::pdf_page_count(const fs::path& pdf) const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:745 · bloque 50
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdfinfo = resolve_tool_path(fs::path{}, "pdfinfo.exe");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!executable_exists(pdfinfo)) return 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:751 · bloque 51
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (!executable_exists(pdfinfo)) return 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:757 · bloque 52
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path staged_pdf = stage_pdf_ascii(pdf, "shared");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:763 · bloque 53
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::vector<std::string> args = {path_utf8(pdfinfo), path_utf8(staged_pdf)};


/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:771 · bloque 54
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const int timeout_ms = env_int_clamped("TLALPOWA_PDFINFO_TIMEOUT_MS", 2500, 600, 20000);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    auto pr = run_command_timed(args, timeout_ms);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 || pr.captured_output.empty()) return 0;
 */
    if (pr.exit_code != 0 || pr.captured_output.empty()) return 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    std::istringstream in(pr.captured_output);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:790 · bloque 55
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string line;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (std::getline(in, line)) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:802 · bloque 56
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        std::string norm = normalize_key(line);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (norm.find("pages:") == 0 || norm.find("paginas:") == 0) {
 */
        if (norm.find("pages:") == 0 || norm.find("paginas:") == 0) {
            std::smatch m;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (std::regex_search(line, m, std::regex(R"(([0-9]+))"))) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:815 · bloque 57
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            if (std::regex_search(line, m, std::regex(R"(([0-9]+))"))) {
                try { return std::max(0, std::stoi(m[1].str())); } catch (...) { return 0; }
            }
        }
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}


/*
 * Función run_pdftotext_bbox: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:839 · bloque 58
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
ProcessResult ExternalTools::run_pdftotext_bbox(const fs::path& pdf, const fs::path& out_html, int first_page, int last_page) const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(out_html.parent_path());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:851 · bloque 59
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path staged_pdf = stage_pdf_ascii(pdf, "shared");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:857 · bloque 60
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const std::string tmp_id = simple_hash_hex(path_utf8(pdf) + "|bbox|" + std::to_string(first_page) + "|" + std::to_string(last_page) + "|" + path_utf8(out_html));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:863 · bloque 61
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path tmp_html = staged_pdf.parent_path() / ("bbox_f" + std::to_string(std::max(1, first_page)) + "_l" + std::to_string(std::max(0, last_page)) + "_" + tmp_id + ".html");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:869 · bloque 62
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdftotext = resolve_tool_path(options_.pdftotext, "pdftotext.exe");
    std::error_code ec;
    fs::remove(tmp_html, ec);


    first_page = std::max(1, first_page);
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    last_page = std::max(0, last_page);

/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:887 · bloque 63
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const int timeout_ms = env_int_clamped("TLALPOWA_BBOX_PROCESS_TIMEOUT_MS", 18000, 2500, 180000);

/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
    auto run_mode = [&](const std::string& mode) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        std::vector<std::string> args = {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:906 · bloque 64
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            path_utf8(pdftotext),
            "-enc", "UTF-8"
        };
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (first_page > 1) {
 */
        if (first_page > 1) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.push_back("-f");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.push_back(std::to_string(first_page));
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (last_page >= first_page) {
 */
        if (last_page >= first_page) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.push_back("-l");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.push_back(std::to_string(last_page));
        }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        args.push_back(mode);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:958 · bloque 65
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        args.push_back(path_utf8(staged_pdf));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        args.push_back(path_utf8(tmp_html));
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        return run_command_timed(args, timeout_ms);
    };

/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
    auto pr = run_mode("-bbox-layout");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 || !fs::exists(tmp_html) || file_size_or_zero(tmp_html) == 0) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:984 · bloque 66
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    if (pr.exit_code != 0 || !fs::exists(tmp_html) || file_size_or_zero(tmp_html) == 0) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:990 · bloque 67
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        ProcessResult first = pr;
        fs::remove(tmp_html, ec);
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
        pr = run_mode("-bbox");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 && !first.captured_output.empty()) {
 */
        if (pr.exit_code != 0 && !first.captured_output.empty()) {
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
            pr.captured_output = "Primer intento -bbox-layout:\n" + first.captured_output + "\nSegundo intento -bbox:\n" + pr.captured_output;
        }
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code == 0 && fs::exists(tmp_html)) {
 */
    if (pr.exit_code == 0 && fs::exists(tmp_html)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1023 · bloque 68
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        copy_file_overwrite(tmp_html, out_html, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
        if (ec) {
            pr.exit_code = 9001;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:1036 · bloque 69
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            pr.captured_output += "\nNo se pudo copiar XHTML temporal a salida real: " + ec.message();
        }
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
    } else if (pr.exit_code != 0 && !pr.captured_output.empty()) {
        pr.command_for_log += "\nSTDERR/STDOUT: " + pr.captured_output;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return pr;
}


/*
 * Función run_pdftotext_layout: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1065 · bloque 70
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
ProcessResult ExternalTools::run_pdftotext_layout(const fs::path& pdf, const fs::path& out_text, int first_page, int last_page) const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(out_text.parent_path());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1077 · bloque 71
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path staged_pdf = stage_pdf_ascii(pdf, "shared");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1083 · bloque 72
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const std::string tmp_id = simple_hash_hex(path_utf8(pdf) + "|layout|" + std::to_string(first_page) + "|" + std::to_string(last_page) + "|" + path_utf8(out_text));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1089 · bloque 73
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path tmp_text = staged_pdf.parent_path() / ("layout_f" + std::to_string(std::max(1, first_page)) + "_l" + std::to_string(std::max(0, last_page)) + "_" + tmp_id + ".txt");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1095 · bloque 74
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdftotext = resolve_tool_path(options_.pdftotext, "pdftotext.exe");
    std::error_code ec;
    fs::remove(tmp_text, ec);

    first_page = std::max(1, first_page);
    last_page = std::max(0, last_page);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1106 · bloque 75
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const int timeout_ms = env_int_clamped("TLALPOWA_LAYOUT_PROCESS_TIMEOUT_MS", 22000, 2500, 240000);

/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
    auto run_mode = [&](const std::string& mode) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1119 · bloque 76
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        std::vector<std::string> args = { path_utf8(pdftotext), "-enc", "UTF-8" };
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (first_page > 1) { args.push_back("-f"); args.push_back(std::to_string(first_page)); }
 */
        if (first_page > 1) { args.push_back("-f"); args.push_back(std::to_string(first_page)); }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (last_page >= first_page) { args.push_back("-l"); args.push_back(std::to_string(last_page)); }
 */
        if (last_page >= first_page) { args.push_back("-l"); args.push_back(std::to_string(last_page)); }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!mode.empty()) args.push_back(mode);
 */
        if (!mode.empty()) args.push_back(mode);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1143 · bloque 77
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        args.push_back(path_utf8(staged_pdf));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        args.push_back(path_utf8(tmp_text));
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1155 · bloque 78
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        return run_command_timed(args, timeout_ms);
    };

/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1158 · bloque 79
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    ProcessResult pr = run_mode("-layout");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 || !fs::exists(tmp_text) || file_size_or_zero(tmp_text) == 0) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1164 · bloque 80
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    if (pr.exit_code != 0 || !fs::exists(tmp_text) || file_size_or_zero(tmp_text) == 0) {
        ProcessResult first = pr;
        fs::remove(tmp_text, ec);
        pr = run_mode("-raw");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 && !first.captured_output.empty()) {
 */
        if (pr.exit_code != 0 && !first.captured_output.empty()) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
            pr.captured_output = "Primer intento -layout:\n" + first.captured_output + "\nSegundo intento -raw:\n" + pr.captured_output;
        }
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code == 0 && fs::exists(tmp_text)) {
 */
    if (pr.exit_code == 0 && fs::exists(tmp_text)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1193 · bloque 81
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        copy_file_overwrite(tmp_text, out_text, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
        if (ec) {
            pr.exit_code = 9003;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · ExternalTools.cpp:1201 · bloque 82
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            pr.captured_output += "\nNo se pudo copiar TXT temporal a salida real: " + ec.message();
        }
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
    } else if (pr.exit_code != 0 && !pr.captured_output.empty()) {
        pr.command_for_log += "\nSTDERR/STDOUT: " + pr.captured_output;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return pr;
}


/*
 * Función run_pdftoppm_pages_png: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1230 · bloque 83
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
ProcessResult ExternalTools::run_pdftoppm_pages_png(const fs::path& pdf, int first_page, int last_page, const fs::path& out_prefix_no_ext) const {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(out_prefix_no_ext.parent_path());
    first_page = std::max(1, first_page);
    last_page = std::max(first_page, last_page);

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1245 · bloque 84
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path staged_pdf = stage_pdf_ascii(pdf, "shared");
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
    const int scale_to = env_int_clamped("TLALPOWA_RENDER_SCALE_TO", 430, 220, 1200);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1257 · bloque 85
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const int timeout_ms = env_int_clamped("TLALPOWA_RENDER_PROCESS_TIMEOUT_MS", 4200, 900, 30000);


/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1265 · bloque 86
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path tmp_prefix = staged_pdf.parent_path() /
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
        ("preview_batch_f" + std::to_string(first_page) + "_l" + std::to_string(last_page));

    std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const std::string tmp_name = path_utf8(tmp_prefix.filename());
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1285 · bloque 87
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    for (const auto& e : fs::directory_iterator(staged_pdf.parent_path(), ec)) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) break;
 */
        if (ec) break;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const std::string fn = path_utf8(e.path().filename());
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fn.find(tmp_name) == 0 && (e.path().extension() == ".png" || e.path().extension() == ".jpg" || e.p...
 */
        if (fn.find(tmp_name) == 0 && (e.path().extension() == ".png" || e.path().extension() == ".jpg" || e.path().extension() == ".jpeg")) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            fs::remove(e.path(), ec);
        }
    }

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1318 · bloque 88
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    auto normalize_outputs = [&](const fs::path& prefix) -> int {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
        int copied = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const std::string prefix_name = path_utf8(prefix.filename());
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1331 · bloque 89
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        static const std::regex page_suffix_re(R"(-0*([0-9]+)\.(png|jpg|jpeg)$)", std::regex_constants::icase);
        std::error_code ec2;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (const auto& e : fs::directory_iterator(prefix.parent_path(), ec2)) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec2) break;
 */
            if (ec2) break;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            const std::string fn = path_utf8(e.path().filename());
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fn.find(prefix_name) != 0) continue;
 */
            if (fn.find(prefix_name) != 0) continue;
            int out_page = 0;
            const std::string suffix = fn.substr(prefix_name.size());
            std::smatch m;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (std::regex_search(suffix, m, page_suffix_re) && m.size() > 1) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1365 · bloque 90
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            if (std::regex_search(suffix, m, page_suffix_re) && m.size() > 1) {
                try { out_page = std::stoi(m[1].str()); } catch (...) { out_page = 0; }
            }


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_page > 0 && out_page < first_page && first_page + out_page - 1 <= last_page) {
 */
            if (out_page > 0 && out_page < first_page && first_page + out_page - 1 <= last_page) {
                out_page = first_page + out_page - 1;
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_page <= 0 && first_page == last_page) out_page = first_page;
 */
            if (out_page <= 0 && first_page == last_page) out_page = first_page;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_page < first_page || out_page > last_page) continue;
 */
            if (out_page < first_page || out_page > last_page) continue;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            fs::path dest = out_prefix_no_ext;
            dest += "-" + std::to_string(out_page) + preview_render_extension();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1402 · bloque 91
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            copy_file_overwrite(e.path(), dest, ec2);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec2) ++copied;
 */
            if (!ec2) ++copied;
        }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return copied;
    };

/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1418 · bloque 92
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    ProcessResult last_error{9002, "renderizador de PDF no disponible", ""};


/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1426 · bloque 93
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdftocairo = resolve_tool_path(fs::path{}, "pdftocairo.exe");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (executable_exists(pdftocairo) && !env_flag_enabled("TLALPOWA_RENDER_DISABLE_CAIRO")) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1432 · bloque 94
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (executable_exists(pdftocairo) && !env_flag_enabled("TLALPOWA_RENDER_DISABLE_CAIRO")) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        std::vector<std::string> args = {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1444 · bloque 95
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            path_utf8(pdftocairo),
            preview_render_jpeg() ? "-jpeg" : "-png",
            "-antialias", "fast",
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
            "-scale-to", std::to_string(scale_to),
            "-f", std::to_string(first_page),
            "-l", std::to_string(last_page),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1460 · bloque 96
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            path_utf8(staged_pdf),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1466 · bloque 97
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            path_utf8(tmp_prefix)
        };
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (preview_render_jpeg()) {
 */
        if (preview_render_jpeg()) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.insert(args.begin() + 2, "-jpegopt");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:1485 · bloque 98
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
            args.insert(args.begin() + 3, "quality=" + std::to_string(env_int_clamped("TLALPOWA_RENDER_JPEG_QUALITY", 38, 18, 92)) + ",optimize=n,progressive=n");
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args.insert(args.begin() + 2, "-gray");
 */
        if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args.insert(args.begin() + 2, "-gray");
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        auto pr = run_command_timed(args, timeout_ms);
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1504 · bloque 99
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        const int copied = normalize_outputs(tmp_prefix);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (copied > 0) {
 */
        if (copied > 0) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_...
 */
            if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_string(pr.exit_code) + "]";
            pr.exit_code = 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return pr;
        }
        last_error = pr;
    }

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1533 · bloque 100
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    const fs::path pdftoppm = resolve_tool_path(options_.pdftoppm, "pdftoppm.exe");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (executable_exists(pdftoppm)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1539 · bloque 101
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (executable_exists(pdftoppm)) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        std::vector<std::string> args = {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1551 · bloque 102
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            path_utf8(pdftoppm),
            preview_render_jpeg() ? "-jpeg" : "-png",
            "-aa", env_flag_enabled("TLALPOWA_RENDER_HIGH_QUALITY_AA") ? "yes" : "no",
            "-aaVector", env_flag_enabled("TLALPOWA_RENDER_HIGH_QUALITY_AA") ? "yes" : "no",
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
            "-scale-to", std::to_string(scale_to),
            "-f", std::to_string(first_page),
            "-l", std::to_string(last_page),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1568 · bloque 103
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            path_utf8(staged_pdf),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            path_utf8(tmp_prefix)
        };
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (preview_render_jpeg()) {
 */
        if (preview_render_jpeg()) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            args.insert(args.begin() + 2, "-jpegopt");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:1593 · bloque 104
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
            args.insert(args.begin() + 3, "quality=" + std::to_string(env_int_clamped("TLALPOWA_RENDER_JPEG_QUALITY", 38, 18, 92)) + ",optimize=n,progressive=n");
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args.insert(args.begin() + 2, "-gray");
 */
        if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args.insert(args.begin() + 2, "-gray");
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        auto pr = run_command_timed(args, timeout_ms);
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1612 · bloque 105
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        int copied = normalize_outputs(tmp_prefix);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (copied > 0) {
 */
        if (copied > 0) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_...
 */
            if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_string(pr.exit_code) + "]";
            pr.exit_code = 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return pr;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0) {
 */
        if (pr.exit_code != 0) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            std::vector<std::string> args_dpi = {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1650 · bloque 106
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
                path_utf8(pdftoppm),
                preview_render_jpeg() ? "-jpeg" : "-png",
                "-aa", env_flag_enabled("TLALPOWA_RENDER_HIGH_QUALITY_AA") ? "yes" : "no",
                "-aaVector", env_flag_enabled("TLALPOWA_RENDER_HIGH_QUALITY_AA") ? "yes" : "no",
                "-r", std::to_string(std::clamp(options_.render_dpi, 10, 80)),
                "-f", std::to_string(first_page),
                "-l", std::to_string(last_page),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1662 · bloque 107
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
                path_utf8(staged_pdf),
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1668 · bloque 108
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                path_utf8(tmp_prefix)
            };
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (preview_render_jpeg()) {
 */
            if (preview_render_jpeg()) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                args_dpi.insert(args_dpi.begin() + 2, "-jpegopt");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:1687 · bloque 109
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
                args_dpi.insert(args_dpi.begin() + 3, "quality=" + std::to_string(env_int_clamped("TLALPOWA_RENDER_JPEG_QUALITY", 38, 18, 92)) + ",optimize=n,progressive=n");
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args_dpi.insert(args_dpi.begin() + 2, "-gray");
 */
            if (env_flag_enabled("TLALPOWA_RENDER_GRAY")) args_dpi.insert(args_dpi.begin() + 2, "-gray");
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1695 · bloque 110
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            const ProcessResult first_try = pr;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
            pr = run_command_timed(args_dpi, timeout_ms);
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · ExternalTools.cpp:1707 · bloque 111
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            copied = normalize_outputs(tmp_prefix);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (copied > 0) {
 */
            if (copied > 0) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_...
 */
                if (pr.exit_code != 0) pr.command_for_log += " [salida parcial util copiada pese a codigo " + std::to_string(pr.exit_code) + "]";
                pr.exit_code = 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
                return pr;
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pr.exit_code != 0 && !first_try.captured_output.empty()) {
 */
            if (pr.exit_code != 0 && !first_try.captured_output.empty()) {
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
                pr.captured_output = "Primer intento -scale-to:\n" + first_try.captured_output + "\nSegundo intento -r:\n" + pr.captured_output;
            }
        }
        last_error = pr;
    }

/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!last_error.captured_output.empty()) last_error.command_for_log += "\nSTDERR/STDOUT: " + last_erro...
 */
    if (!last_error.captured_output.empty()) last_error.command_for_log += "\nSTDERR/STDOUT: " + last_error.captured_output;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return last_error;
}


/*
 * Función run_pdftoppm_page_png: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1770 · bloque 112
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
ProcessResult ExternalTools::run_pdftoppm_page_png(const fs::path& pdf, int page, const fs::path& out_prefix_no_ext) const {


/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · ExternalTools.cpp:1778 · bloque 113
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    return run_pdftoppm_pages_png(pdf, page, page, out_prefix_no_ext);
}

/*
 * Función quote_for_log: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1791 · bloque 114
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
std::string ExternalTools::quote_for_log(const std::string& s) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s.find_first_of(" \t\"'&()[]{}áéíóúÁÉÍÓÚñÑ") == std::string::npos) return s;
 */
    if (s.find_first_of(" \t\"'&()[]{}áéíóúÁÉÍÓÚñÑ") == std::string::npos) return s;
    std::string out = "\"";
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1804 · bloque 115
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    for (char c : s) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c == '"') out += "\\\"";
 */
        if (c == '"') out += "\\\"";
        else out += c;
    }
    out += "\"";
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return out;
}

/*
 * Función run_command: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1832 · bloque 116
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
ProcessResult ExternalTools::run_command(const std::vector<std::string>& args) const {
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    return run_command_timed(args, 0);
}


/*
 * Función run_command_timed: orquesta una operación completa con límites claros de inicialización, progreso, cancelación, efectos laterales y recuperación ante error.
 * Contexto: pertenece a adaptadores hacia herramientas externas y procesos auxiliares: ejecución controlada, conversión, rasterización, saneamiento de argumentos y lectura de resultados. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1852 · bloque 117
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
ProcessResult ExternalTools::run_command_timed(const std::vector<std::string>& args, int timeout_ms) const {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (args.empty()) return {-1, "", ""};
 */
    if (args.empty()) return {-1, "", ""};


/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1861 · bloque 118
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    ExternalProcessSlot process_slot(poppler_process_budget());
    std::ostringstream log;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < args.size(); ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (i) log << ' ';
 */
        if (i) log << ' ';
        log << quote_for_log(args[i]);
    }
    const std::string command_log = log.str();
#ifdef _WIN32
    std::wstring cmdline;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < args.size(); ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (i) cmdline += L" ";
 */
        if (i) cmdline += L" ";
        std::wstring w = widen_utf8(args[i]);
        cmdline += L"\"";
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (wchar_t c : w) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c == L'\"') cmdline += L"\\\"";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1905 · bloque 119
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            if (c == L'\"') cmdline += L"\\\"";
            else cmdline += c;
        }
        cmdline += L"\"";
    }

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    HANDLE readPipe = nullptr;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    HANDLE writePipe = nullptr;
    std::string captured;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) {
 */
    if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return {static_cast<int>(GetLastError()), command_log + " [CreatePipe failed]", ""};
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1948 · bloque 120
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    si.hStdOutput = writePipe;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    si.hStdError = writePipe;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno.
 */
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<wchar_t> mutable_cmd(cmdline.begin(), cmdline.end());
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    mutable_cmd.push_back(L'\0');
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:1983 · bloque 121
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    DWORD priority_flags = CREATE_NO_WINDOW | ABOVE_NORMAL_PRIORITY_CLASS;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env_flag_enabled("TLALPOWA_POPPLER_HIGH_PRIORITY")) priority_flags = CREATE_NO_WINDOW | HIGH_PRIOR...
 */
    if (env_flag_enabled("TLALPOWA_POPPLER_HIGH_PRIORITY")) priority_flags = CREATE_NO_WINDOW | HIGH_PRIORITY_CLASS;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:1990 · bloque 122
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    BOOL ok = CreateProcessW(nullptr, mutable_cmd.data(), nullptr, nullptr, TRUE, priority_flags, nullptr, nullptr, &si, &pi);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    CloseHandle(writePipe);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ok) {
 */
    if (!ok) {
        const DWORD err = GetLastError();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        CloseHandle(readPipe);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:2015 · bloque 123
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        return {static_cast<int>(err), command_log + " [CreateProcessW failed]", ""};
    }

/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    const auto t0 = std::chrono::steady_clock::now();
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    bool timed_out = false;
    char buffer[4096];
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (;;) {


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (timeout_ms > 0 &&
 */
        if (timeout_ms > 0 &&
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count() >= timeout_ms) {
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
            timed_out = true;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:2057 · bloque 124
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            TerminateProcess(pi.hProcess, 1460);
            WaitForSingleObject(pi.hProcess, 500);
            break;
        }

        DWORD available = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (PeekNamedPipe(readPipe, nullptr, 0, nullptr, &available, nullptr) && available > 0) {
 */
        if (PeekNamedPipe(readPipe, nullptr, 0, nullptr, &available, nullptr) && available > 0) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            DWORD read = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ReadFile(readPipe, buffer, static_cast<DWORD>(sizeof(buffer)), &read, nullptr) && read > 0) {
 */
            if (ReadFile(readPipe, buffer, static_cast<DWORD>(sizeof(buffer)), &read, nullptr) && read > 0) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · ExternalTools.cpp:2086 · bloque 125
 * Este tramo pertenece a frontera con procesos externos y utilidades del sistema operativo, donde un ejecutable auxiliar nunca debe bloquear ni corromper una importación; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                captured.append(buffer, buffer + read);


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (captured.size() > 65536) captured.erase(0, captured.size() - 65536);
 */
                if (captured.size() > 65536) captured.erase(0, captured.size() - 65536);
            }
        } else {
            DWORD code = STILL_ACTIVE;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:2098 · bloque 126
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            GetExitCodeProcess(pi.hProcess, &code);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (code != STILL_ACTIVE) break;
 */
            if (code != STILL_ACTIVE) break;
            Sleep(12);
        }
    }
    DWORD code = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · ExternalTools.cpp:2109 · bloque 127
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    GetExitCodeProcess(pi.hProcess, &code);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · ExternalTools.cpp:2115 · bloque 128
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    CloseHandle(readPipe);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (timed_out) {
 */
    if (timed_out) {
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
        return {1460, command_log + " [timeout " + std::to_string(timeout_ms) + " ms; proceso terminado para no bloquear importacion]", captured};
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return {static_cast<int>(code), command_log, captured};
#else
    int code = std::system(command_log.c_str());
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la mediación con herramientas externas, donde cada proceso o binario debe aislarse para que fallas de entorno no corrompan el estado analítico interno, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return {code, command_log, ""};
#endif
}

}
