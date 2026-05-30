/*
 * Archivo: Ixiptlah.cpp
 * Papel técnico: serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:7 · bloque 1
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
#include "Ixiptlah.hpp"

#include <chrono>
#include <cstring>
#include <limits>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:16 · bloque 2
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
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:20 · bloque 3
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace {

/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:27 · bloque 4
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
constexpr char kIxiptlahMagic[8] = {'I','X','I','P','T','L','A','H'};
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:33 · bloque 5
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
constexpr std::uint32_t kIxiptlahFileVersionLegacy = 2;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:39 · bloque 6
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
constexpr std::uint32_t kIxiptlahFileVersion = 3;
constexpr std::uint64_t kMaxPayloadBytes = 512ull * 1024ull * 1024ull;
constexpr std::uint32_t kCodecRaw = 0;
constexpr std::uint32_t kCodecIxLz = 1;


/*
 * Tipo IxiptlahRecordEnvelope: agrupa estado o registros usados por serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:54 · bloque 7
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahRecordEnvelope {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:60 · bloque 8
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahRecordType type = IxiptlahRecordType::EpidemiologyObservation;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:66 · bloque 9
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    std::uint32_t schema = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
    std::uint64_t stored_size = 0;
    std::uint64_t raw_size = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:74 · bloque 10
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::uint32_t codec = kCodecRaw;
};

/*
 * Tipo IxiptlahSink: agrupa estado o registros usados por serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:86 · bloque 11
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahSink {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path path;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:98 · bloque 12
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ofstream stream;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:104 · bloque 13
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::uint32_t version = kIxiptlahFileVersion;
    std::uint64_t last_used = 0;
};

/*
 * Tipo IxiptlahIndexedRecord: agrupa estado o registros usados por serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:117 · bloque 14
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahIndexedRecord {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:123 · bloque 15
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahRecordType type = IxiptlahRecordType::EpidemiologyObservation;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:129 · bloque 16
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    std::uint32_t schema = 0;
    std::uint64_t payload_offset = 0;
    std::uint64_t stored_size = 0;
    std::uint64_t raw_size = 0;
    std::uint32_t codec = kCodecRaw;
};

/*
 * Tipo IxiptlahFileIndex: agrupa estado o registros usados por serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:145 · bloque 17
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahFileIndex {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:151 · bloque 18
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::uint64_t file_size = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    std::int64_t mtime_ns = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::uint32_t file_version = 0;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:169 · bloque 19
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::vector<IxiptlahIndexedRecord> records;
    bool valid = false;
};


/*
 * Función ixiptlah_mu: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:184 · bloque 20
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::mutex& ixiptlah_mu() {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · Ixiptlah.cpp:190 · bloque 21
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    static std::mutex mu;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return mu;
}

/*
 * Función ixiptlah_sinks: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:209 · bloque 22
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::unordered_map<std::string, std::unique_ptr<IxiptlahSink>>& ixiptlah_sinks() {
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:215 · bloque 23
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    static std::unordered_map<std::string, std::unique_ptr<IxiptlahSink>> sinks;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return sinks;
}

/*
 * Función ixiptlah_tick: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:234 · bloque 24
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::uint64_t& ixiptlah_tick() {
    static std::uint64_t tick = 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return tick;
}

/*
 * Función ixiptlah_index_mu: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:254 · bloque 25
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::mutex& ixiptlah_index_mu() {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · Ixiptlah.cpp:260 · bloque 26
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    static std::mutex mu;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return mu;
}

/*
 * Función ixiptlah_index_cache: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:279 · bloque 27
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::unordered_map<std::string, IxiptlahFileIndex>& ixiptlah_index_cache() {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:285 · bloque 28
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    static std::unordered_map<std::string, IxiptlahFileIndex> cache;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return cache;
}

/*
 * Función ixiptlah_mtime_ns: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:304 · bloque 29
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::int64_t ixiptlah_mtime_ns(const fs::path& path) {
    std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:311 · bloque 30
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    const auto t = fs::last_write_time(path, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) return 0;
 */
    if (ec) return 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
}

/*
 * Función ixiptlah_invalidate_index_cache: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:336 · bloque 31
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_invalidate_index_cache(const fs::path& path) {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:342 · bloque 32
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_index_mu());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:348 · bloque 33
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    ixiptlah_index_cache().erase(path_utf8(path));
}

/*
 * Función env_truthy_ix: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
bool env_truthy_ix(const char* name) {
    const std::string v = getenv_utf8_or_empty(name);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v.empty()) return false;
 */
    if (v.empty()) return false;
    const char c = v.front();
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return c == '1' || c == 's' || c == 'S' || c == 't' || c == 'T' || c == 'y' || c == 'Y';
}

/*
 * Función ixiptlah_default_write_version: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:388 · bloque 34
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::uint32_t ixiptlah_default_write_version() {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:394 · bloque 35
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    return env_truthy_ix("TLALPOWA_IXIPTLAH_FORCE_V2") ? kIxiptlahFileVersionLegacy : kIxiptlahFileVersion;
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:402 · bloque 36
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_file_header(std::ostream& out, std::uint32_t version = kIxiptlahFileVersion) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:408 · bloque 37
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    out.write(kIxiptlahMagic, sizeof(kIxiptlahMagic));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:414 · bloque 38
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    return ixiptlah_write_value(out, version);
}

/*
 * Función ixiptlah_read_file_header: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:427 · bloque 39
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_read_file_header(std::istream& in, std::uint32_t& version) {
    char magic[8] = {};
    version = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    in.read(magic, sizeof(magic));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in || std::memcmp(magic, kIxiptlahMagic, sizeof(kIxiptlahMagic)) != 0) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:441 · bloque 40
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!in || std::memcmp(magic, kIxiptlahMagic, sizeof(kIxiptlahMagic)) != 0) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, version)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:447 · bloque 41
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_read_value(in, version)) return false;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:453 · bloque 42
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    return version == kIxiptlahFileVersionLegacy || version == kIxiptlahFileVersion;
}

/*
 * Función ixiptlah_existing_version_or_default: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:466 · bloque 43
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::uint32_t ixiptlah_existing_version_or_default(const fs::path& path, bool needs_header) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (needs_header) return ixiptlah_default_write_version();
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:472 · bloque 44
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (needs_header) return ixiptlah_default_write_version();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:478 · bloque 45
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(path, std::ios::binary);
    std::uint32_t version = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (in && ixiptlah_read_file_header(in, version)) return version;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:485 · bloque 46
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (in && ixiptlah_read_file_header(in, version)) return version;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:491 · bloque 47
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    return ixiptlah_default_write_version();
}


/*
 * Función ix_lz_compress: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
std::string ix_lz_compress(const std::string& input) {
    const size_t n = input.size();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n < 48) return {};
 */
    if (n < 48) return {};
    std::string out;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    out.reserve(n);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<int> last(1u << 16, -1);
    size_t i = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:532 · bloque 48
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    while (i < n) {
        const size_t control_pos = out.size();
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        out.push_back('\0');
        unsigned char control = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (int bit = 0; bit < 8 && i < n; ++bit) {
            size_t best_len = 0;
            size_t best_dist = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (i + 4 <= n) {
 */
            if (i + 4 <= n) {
                const unsigned char a = static_cast<unsigned char>(input[i]);
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
                const unsigned char b = static_cast<unsigned char>(input[i + 1]);
                const unsigned char c = static_cast<unsigned char>(input[i + 2]);
                const std::uint32_t h = ((static_cast<std::uint32_t>(a) * 251u) ^
                                         (static_cast<std::uint32_t>(b) * 911u) ^
                                         (static_cast<std::uint32_t>(c) * 3571u)) & 0xffffu;
                const int prev = last[h];
                last[h] = static_cast<int>(i);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (prev >= 0 && i > static_cast<size_t>(prev)) {
 */
                if (prev >= 0 && i > static_cast<size_t>(prev)) {
                    const size_t dist = i - static_cast<size_t>(prev);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dist <= 65535) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:580 · bloque 49
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                    if (dist <= 65535) {
                        size_t len = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
                        while (i + len < n && static_cast<size_t>(prev) + len < i &&
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
                               input[static_cast<size_t>(prev) + len] == input[i + len] && len < 255 + 4) {
                            ++len;
                        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (len >= 4) {
 */
                        if (len >= 4) {
                            best_len = len;
                            best_dist = dist;
                        }
                    }
                }
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (best_len >= 4) {
 */
            if (best_len >= 4) {
                control |= static_cast<unsigned char>(1u << bit);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                out.push_back(static_cast<char>(best_dist & 0xffu));
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                out.push_back(static_cast<char>((best_dist >> 8) & 0xffu));
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                out.push_back(static_cast<char>(best_len - 4));
                const size_t end = std::min(n, i + best_len);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
                for (size_t j = i + 1; j + 2 < end; ++j) {
                    const unsigned char a = static_cast<unsigned char>(input[j]);
                    const unsigned char b = static_cast<unsigned char>(input[j + 1]);
                    const unsigned char c = static_cast<unsigned char>(input[j + 2]);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:643 · bloque 50
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                    const std::uint32_t h = ((static_cast<std::uint32_t>(a) * 251u) ^
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
                                             (static_cast<std::uint32_t>(b) * 911u) ^
                                             (static_cast<std::uint32_t>(c) * 3571u)) & 0xffffu;
                    last[h] = static_cast<int>(j);
                }
                i += best_len;
            } else {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                out.push_back(input[i++]);
            }
        }
        out[control_pos] = static_cast<char>(control);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out.size() >= n) return {};
 */
        if (out.size() >= n) return {};
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return out;
}

/*
 * Función ix_lz_decompress: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
bool ix_lz_decompress(const std::string& input, std::uint64_t raw_size, std::string& output) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (raw_size > kMaxPayloadBytes) return false;
 */
    if (raw_size > kMaxPayloadBytes) return false;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    output.clear();
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    output.reserve(static_cast<size_t>(raw_size));
    size_t i = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (i < input.size() && output.size() < raw_size) {
        const unsigned char control = static_cast<unsigned char>(input[i++]);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (int bit = 0; bit < 8 && output.size() < raw_size; ++bit) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (control & static_cast<unsigned char>(1u << bit)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:727 · bloque 51
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            if (control & static_cast<unsigned char>(1u << bit)) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (i + 3 > input.size()) return false;
 */
                if (i + 3 > input.size()) return false;
                const std::uint32_t dist = static_cast<unsigned char>(input[i]) |
                    (static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1])) << 8);
                const std::uint32_t len = static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 2])) + 4u;
                i += 3;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dist == 0 || dist > output.size()) return false;
 */
                if (dist == 0 || dist > output.size()) return false;
                const size_t start = output.size() - dist;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
                for (std::uint32_t j = 0; j < len; ++j) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (output.size() >= raw_size) break;
 */
                    if (output.size() >= raw_size) break;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                    output.push_back(output[start + j]);
                }
            } else {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (i >= input.size()) return false;
 */
                if (i >= input.size()) return false;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                output.push_back(input[i++]);
            }
        }
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return output.size() == raw_size;
}

/*
 * Función ixiptlah_payload_for_storage: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:798 · bloque 52
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::string ixiptlah_payload_for_storage(const std::string& bytes, std::uint32_t& codec) {
    codec = kCodecRaw;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (bytes.size() < 48 || env_truthy_ix("TLALPOWA_IXIPTLAH_DISABLE_COMPRESSION")) return bytes;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:805 · bloque 53
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (bytes.size() < 48 || env_truthy_ix("TLALPOWA_IXIPTLAH_DISABLE_COMPRESSION")) return bytes;
    std::string packed = ix_lz_compress(bytes);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!packed.empty() && packed.size() + 8 < bytes.size()) {
 */
    if (!packed.empty() && packed.size() + 8 < bytes.size()) {
        codec = kCodecIxLz;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return packed;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return bytes;
}

/*
 * Función ixiptlah_decode_payload: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:839 · bloque 54
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_decode_payload(std::string stored, const IxiptlahRecordEnvelope& env, std::string& decoded) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.codec == kCodecRaw) {
 */
    if (env.codec == kCodecRaw) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.raw_size != env.stored_size) return false;
 */
        if (env.raw_size != env.stored_size) return false;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
        decoded = std::move(stored);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return true;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.codec == kCodecIxLz) {
 */
    if (env.codec == kCodecIxLz) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return ix_lz_decompress(stored, env.raw_size, decoded);
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return false;
}

/*
 * Función ixiptlah_read_record_envelope: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:896 · bloque 55
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_read_record_envelope(std::istream& in, std::uint32_t file_version, IxiptlahRecordEnvelope& env) {
    std::uint32_t raw_type = 0;
    env = {};
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, raw_type)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:904 · bloque 56
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_read_value(in, raw_type)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, env.schema)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:910 · bloque 57
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_read_value(in, env.schema)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, env.stored_size)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:916 · bloque 58
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_read_value(in, env.stored_size)) return false;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:922 · bloque 59
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    env.type = static_cast<IxiptlahRecordType>(raw_type);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (file_version == kIxiptlahFileVersionLegacy) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:928 · bloque 60
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (file_version == kIxiptlahFileVersionLegacy) {
        env.raw_size = env.stored_size;
        env.codec = kCodecRaw;
    } else {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, env.raw_size)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:937 · bloque 61
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_read_value(in, env.raw_size)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, env.codec)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:943 · bloque 62
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_read_value(in, env.codec)) return false;
        std::uint32_t reserved = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, reserved)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:950 · bloque 63
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_read_value(in, reserved)) return false;
        (void)reserved;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.stored_size > kMaxPayloadBytes || env.raw_size > kMaxPayloadBytes) return false;
 */
    if (env.stored_size > kMaxPayloadBytes || env.raw_size > kMaxPayloadBytes) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.codec == kCodecRaw && env.stored_size != env.raw_size) return false;
 */
    if (env.codec == kCodecRaw && env.stored_size != env.raw_size) return false;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return true;
}

/*
 * Función ixiptlah_scan_file_index: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:983 · bloque 64
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_scan_file_index(const fs::path& path, IxiptlahFileIndex& index) {
    index = {};
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:990 · bloque 65
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const std::uint64_t size = file_size_or_zero(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (size == 0) return false;
 */
    if (size == 0) return false;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1002 · bloque 66
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(path, std::ios::binary);
    std::uint32_t version = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in || !ixiptlah_read_file_header(in, version)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1009 · bloque 67
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!in || !ixiptlah_read_file_header(in, version)) return false;

/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (;;) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1022 · bloque 68
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        IxiptlahRecordEnvelope env;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_record_envelope(in, version, env)) break;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1028 · bloque 69
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_read_record_envelope(in, version, env)) break;
        const auto payload_pos = in.tellg();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (payload_pos < std::streampos(0)) break;
 */
        if (payload_pos < std::streampos(0)) break;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.stored_size > static_cast<std::uint64_t>(std::numeric_limits<std::streamoff>::max())) break;
 */
        if (env.stored_size > static_cast<std::uint64_t>(std::numeric_limits<std::streamoff>::max())) break;
        const auto payload_offset = static_cast<std::uint64_t>(payload_pos);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (payload_offset > size) break;
 */
        if (payload_offset > size) break;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (env.stored_size > size - payload_offset) break;
 */
        if (env.stored_size > size - payload_offset) break;

/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1061 · bloque 70
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        IxiptlahIndexedRecord rec;
        rec.type = env.type;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:1068 · bloque 71
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        rec.schema = env.schema;
        rec.payload_offset = payload_offset;
        rec.stored_size = env.stored_size;
        rec.raw_size = env.raw_size;
        rec.codec = env.codec;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
        index.records.push_back(rec);

        in.seekg(static_cast<std::streamoff>(env.stored_size), std::ios::cur);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) break;
 */
        if (!in) break;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (static_cast<std::uint64_t>(in.tellg()) > size) break;
 */
        if (static_cast<std::uint64_t>(in.tellg()) > size) break;
    }

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1100 · bloque 72
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    index.file_size = size;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1106 · bloque 73
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    index.mtime_ns = ixiptlah_mtime_ns(path);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    index.file_version = version;
    index.valid = true;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return true;
}

/*
 * Función ixiptlah_index_for_path: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1132 · bloque 74
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
IxiptlahFileIndex ixiptlah_index_for_path(const fs::path& path) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const std::string key = path_utf8(path);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1144 · bloque 75
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const std::uint64_t size = file_size_or_zero(path);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1150 · bloque 76
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::int64_t mtime = ixiptlah_mtime_ns(path);
    {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1157 · bloque 77
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        std::lock_guard<std::mutex> lock(ixiptlah_index_mu());
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1163 · bloque 78
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        const auto it = ixiptlah_index_cache().find(key);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it != ixiptlah_index_cache().end() && it->second.valid &&
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1169 · bloque 79
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (it != ixiptlah_index_cache().end() && it->second.valid &&
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1175 · bloque 80
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
            it->second.file_size == size && it->second.mtime_ns == mtime) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1181 · bloque 81
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            return it->second;
        }
    }

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1190 · bloque 82
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahFileIndex fresh;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1196 · bloque 83
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    ixiptlah_scan_file_index(path, fresh);
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1202 · bloque 84
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_index_mu());
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1208 · bloque 85
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    auto& slot = ixiptlah_index_cache()[key];
    slot = fresh;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return slot;
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1223 · bloque 86
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_record(std::ostream& out,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                           std::uint32_t file_version,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1235 · bloque 87
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                           IxiptlahRecordType type,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:1241 · bloque 88
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
                           std::uint32_t schema_version,
/*
 * Función ixiptlah_write_record: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                           const std::string& bytes) {
    const std::uint32_t raw_type = static_cast<std::uint32_t>(type);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (file_version <= kIxiptlahFileVersionLegacy) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1254 · bloque 89
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (file_version <= kIxiptlahFileVersionLegacy) {
        const std::uint64_t payload_size = static_cast<std::uint64_t>(bytes.size());
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_write_value(out, raw_type) ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1261 · bloque 90
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_write_value(out, raw_type) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1267 · bloque 91
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            !ixiptlah_write_value(out, schema_version) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1273 · bloque 92
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            !ixiptlah_write_value(out, payload_size)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (payload_size > 0) out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1279 · bloque 93
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (payload_size > 0) out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return static_cast<bool>(out);
    }
    std::uint32_t codec = kCodecRaw;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1293 · bloque 94
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::string stored = ixiptlah_payload_for_storage(bytes, codec);
    const std::uint64_t stored_size = static_cast<std::uint64_t>(stored.size());
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
    const std::uint64_t raw_size = static_cast<std::uint64_t>(bytes.size());
    const std::uint32_t reserved = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_write_value(out, raw_type) ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1307 · bloque 95
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_write_value(out, raw_type) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1313 · bloque 96
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, schema_version) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1319 · bloque 97
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, stored_size) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1325 · bloque 98
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, raw_size) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1331 · bloque 99
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, codec) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1337 · bloque 100
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, reserved)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (stored_size > 0) out.write(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
    if (stored_size > 0) out.write(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return static_cast<bool>(out);
}

/*
 * Función ixiptlah_write_indexed_record_raw: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1362 · bloque 101
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_indexed_record_raw(std::ostream& out, const IxiptlahIndexedRecord& rec, const std::string& stored) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (stored.size() != rec.stored_size) return false;
 */
    if (stored.size() != rec.stored_size) return false;
    const std::uint32_t raw_type = static_cast<std::uint32_t>(rec.type);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1370 · bloque 102
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    const std::uint32_t reserved = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_write_value(out, raw_type) ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1376 · bloque 103
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_write_value(out, raw_type) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1382 · bloque 104
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, rec.schema) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1388 · bloque 105
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, rec.stored_size) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1394 · bloque 106
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, rec.raw_size) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1400 · bloque 107
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, rec.codec) ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1406 · bloque 108
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        !ixiptlah_write_value(out, reserved)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!stored.empty()) out.write(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
    if (!stored.empty()) out.write(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return static_cast<bool>(out);
}

/*
 * Función ixiptlah_sink_for_path_locked: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1431 · bloque 109
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
IxiptlahSink* ixiptlah_sink_for_path_locked(const fs::path& path) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (path.empty()) return nullptr;
 */
    if (path.empty()) return nullptr;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(path.parent_path());
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1449 · bloque 110
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    auto& sinks = ixiptlah_sinks();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const std::string key = path_utf8(path);
    auto it = sinks.find(key);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it != sinks.end()) {
 */
    if (it != sinks.end()) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1468 · bloque 111
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        it->second->last_used = ++ixiptlah_tick();
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return it->second->stream ? it->second.get() : nullptr;
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1481 · bloque 112
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    const bool needs_header = !fs::exists(path) || file_size_or_zero(path) == 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1487 · bloque 113
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::uint32_t version = ixiptlah_existing_version_or_default(path, needs_header);
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1493 · bloque 114
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    auto sink = std::make_unique<IxiptlahSink>();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    sink->path = path;
    sink->version = version;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1506 · bloque 115
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    sink->last_used = ++ixiptlah_tick();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    sink->stream.open(path, std::ios::binary | std::ios::app);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!sink->stream) return nullptr;
 */
    if (!sink->stream) return nullptr;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (needs_header && !ixiptlah_write_file_header(sink->stream, version)) return nullptr;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1524 · bloque 116
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (needs_header && !ixiptlah_write_file_header(sink->stream, version)) return nullptr;
    auto* out_sink = sink.get();
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    sinks.emplace(key, std::move(sink));
    constexpr size_t kMaxOpenSinks = 96;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (sinks.size() > kMaxOpenSinks) {
 */
    if (sinks.size() > kMaxOpenSinks) {
        auto victim = sinks.end();
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (auto jt = sinks.begin(); jt != sinks.end(); ++jt) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (jt->first == key) continue;
 */
            if (jt->first == key) continue;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (victim == sinks.end() || jt->second->last_used < victim->second->last_used) victim = jt;
 */
            if (victim == sinks.end() || jt->second->last_used < victim->second->last_used) victim = jt;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (victim != sinks.end()) {
 */
        if (victim != sinks.end()) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:1565 · bloque 117
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            victim->second->stream.flush();
            victim->second->stream.close();
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            sinks.erase(victim);
        }
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return out_sink;
}

}

/*
 * Función ixiptlah_path: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1595 · bloque 118
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
fs::path ixiptlah_path(const fs::path& root, const std::string& stem) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1601 · bloque 119
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    return root / (stem + kIxiptlahExtension);
}

/*
 * Función ixiptlah_write_string: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1614 · bloque 120
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_string(std::ostream& out, const std::string& value) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (value.size() > static_cast<size_t>(std::numeric_limits<std::uint32_t>::max())) return false;
 */
    if (value.size() > static_cast<size_t>(std::numeric_limits<std::uint32_t>::max())) return false;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
    const std::uint32_t n = static_cast<std::uint32_t>(value.size());
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_write_value(out, n)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1632 · bloque 121
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_write_value(out, n)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n > 0) out.write(value.data(), static_cast<std::streamsize>(n));
 */
    if (n > 0) out.write(value.data(), static_cast<std::streamsize>(n));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return static_cast<bool>(out);
}

/*
 * Función ixiptlah_read_string: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1657 · bloque 122
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_read_string(std::istream& in, std::string& value) {
    std::uint32_t n = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_read_value(in, n)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1664 · bloque 123
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (!ixiptlah_read_value(in, n)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n > 64u * 1024u * 1024u) return false;
 */
    if (n > 64u * 1024u * 1024u) return false;
    value.assign(n, '\0');
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n > 0) in.read(value.data(), static_cast<std::streamsize>(n));
 */
    if (n > 0) in.read(value.data(), static_cast<std::streamsize>(n));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1683 · bloque 124
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    return static_cast<bool>(in);
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1691 · bloque 125
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_append_record(const fs::path& path,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1697 · bloque 126
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                           IxiptlahRecordType type,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:1703 · bloque 127
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
                           std::uint32_t schema_version,
/*
 * Función ixiptlah_append_record: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                           const std::function<bool(std::ostream&)>& write_payload) {
    std::ostringstream payload(std::ios::out | std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!write_payload(payload)) return false;
 */
    if (!write_payload(payload)) return false;
    const std::string bytes = payload.str();
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1728 · bloque 128
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1734 · bloque 129
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahSink* sink = ixiptlah_sink_for_path_locked(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!sink || !sink->stream) return false;
 */
    if (!sink || !sink->stream) return false;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1746 · bloque 130
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const bool ok = ixiptlah_write_record(sink->stream, sink->version, type, schema_version, bytes);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ok) {
 */
    if (ok) {


/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:1755 · bloque 131
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        sink->stream.flush();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!sink->stream) return false;
 */
        if (!sink->stream) return false;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1767 · bloque 132
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        ixiptlah_invalidate_index_cache(path);
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return ok;
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1782 · bloque 133
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_single_record_atomic(const fs::path& path,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1788 · bloque 134
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                                        IxiptlahRecordType type,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:1794 · bloque 135
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
                                        std::uint32_t schema_version,
/*
 * Función ixiptlah_write_single_record_atomic: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                        const std::function<bool(std::ostream&)>& write_payload) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (path.empty()) return false;
 */
    if (path.empty()) return false;
    std::ostringstream payload(std::ios::out | std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!write_payload(payload)) return false;
 */
    if (!write_payload(payload)) return false;
    const std::string bytes = payload.str();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(path.parent_path());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path tmp_path = fs::path(path.wstring() + L".tmp");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1837 · bloque 136
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::uint32_t version = ixiptlah_default_write_version();
    {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:1844 · bloque 137
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        std::ofstream out(tmp_path, std::ios::binary | std::ios::trunc);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out || !ixiptlah_write_file_header(out, version)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1850 · bloque 138
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!out || !ixiptlah_write_file_header(out, version)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_write_record(out, version, type, schema_version, bytes)) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1856 · bloque 139
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_write_record(out, version, type, schema_version, bytes)) return false;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return false;
 */
        if (!out) return false;
    }
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1869 · bloque 140
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1875 · bloque 141
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    auto& sinks = ixiptlah_sinks();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const std::string key = path_utf8(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (auto it = sinks.find(key); it != sinks.end()) {
 */
    if (auto it = sinks.find(key); it != sinks.end()) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it->second && it->second->stream) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1893 · bloque 142
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (it->second && it->second->stream) {
            it->second->stream.flush();
            it->second->stream.close();
        }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        sinks.erase(it);
    }
    std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:1910 · bloque 143
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    fs::rename(tmp_path, path, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
    if (ec) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        fs::remove(path, ec);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:1934 · bloque 144
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        fs::rename(tmp_path, path, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
        if (ec) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            fs::remove(tmp_path, ec);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return false;
        }
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1960 · bloque 145
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    ixiptlah_invalidate_index_cache(path);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return true;
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1974 · bloque 146
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_read_records(const fs::path& path,
/*
 * Función ixiptlah_read_records: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1985 · bloque 147
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                          const std::function<bool(IxiptlahRecordType, std::uint32_t, std::istream&)>& read_payload) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:1991 · bloque 148
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const IxiptlahFileIndex index = ixiptlah_index_for_path(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!index.valid) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:1997 · bloque 149
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    if (!index.valid) return;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2003 · bloque 150
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(path, std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) return;
 */
    if (!in) return;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2015 · bloque 151
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    for (const IxiptlahIndexedRecord& rec : index.records) {


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) continue;
 */
        if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) continue;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        in.clear();
        in.seekg(static_cast<std::streamoff>(rec.payload_offset), std::ios::beg);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) continue;
 */
        if (!in) continue;
        std::string stored(static_cast<size_t>(rec.stored_size), '\0');
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
        if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) continue;
 */
        if (!in) continue;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2055 · bloque 152
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        IxiptlahRecordEnvelope env;
        env.type = rec.type;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2062 · bloque 153
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        env.schema = rec.schema;
        env.stored_size = rec.stored_size;
        env.raw_size = rec.raw_size;
        env.codec = rec.codec;
        std::string decoded;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_decode_payload(std::move(stored), env, decoded)) continue;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2072 · bloque 154
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_decode_payload(std::move(stored), env, decoded)) continue;
        std::istringstream payload_in(decoded, std::ios::in | std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!read_payload(rec.type, rec.schema, payload_in)) break;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2079 · bloque 155
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        if (!read_payload(rec.type, rec.schema, payload_in)) break;
    }
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2088 · bloque 156
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_read_selected_records(const fs::path& path,
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2094 · bloque 157
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                                   const std::function<bool(IxiptlahRecordType, std::uint32_t)>& accept_record,
/*
 * Función ixiptlah_read_selected_records: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2105 · bloque 158
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                                   const std::function<bool(IxiptlahRecordType, std::uint32_t, std::istream&)>& read_payload) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2111 · bloque 159
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const IxiptlahFileIndex index = ixiptlah_index_for_path(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!index.valid) return;
 */
    if (!index.valid) return;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2123 · bloque 160
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(path, std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) return;
 */
    if (!in) return;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2135 · bloque 161
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    for (const IxiptlahIndexedRecord& rec : index.records) {
        bool accepted = true;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2142 · bloque 162
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        try { accepted = !accept_record || accept_record(rec.type, rec.schema); } catch (...) { accepted = false; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!accepted) continue;
 */
        if (!accepted) continue;


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) continue;
 */
        if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) continue;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        in.clear();
        in.seekg(static_cast<std::streamoff>(rec.payload_offset), std::ios::beg);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) continue;
 */
        if (!in) continue;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:2170 · bloque 163
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        std::string stored(static_cast<size_t>(rec.stored_size), '\0');
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
        if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) continue;
 */
        if (!in) continue;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2188 · bloque 164
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        IxiptlahRecordEnvelope env;
        env.type = rec.type;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2195 · bloque 165
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        env.schema = rec.schema;
        env.stored_size = rec.stored_size;
        env.raw_size = rec.raw_size;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
        env.codec = rec.codec;
        std::string decoded;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ixiptlah_decode_payload(std::move(stored), env, decoded)) continue;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2210 · bloque 166
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!ixiptlah_decode_payload(std::move(stored), env, decoded)) continue;
        std::istringstream payload_in(decoded, std::ios::in | std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!read_payload(rec.type, rec.schema, payload_in)) break;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2217 · bloque 167
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        if (!read_payload(rec.type, rec.schema, payload_in)) break;
    }
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2226 · bloque 168
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
IxiptlahRewriteStats ixiptlah_rewrite_without_records(
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& path,
/*
 * Función ixiptlah_rewrite_without_records: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2243 · bloque 169
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::function<bool(IxiptlahRecordType, std::uint32_t)>& drop_record) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2249 · bloque 170
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahRewriteStats stats;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2255 · bloque 171
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const IxiptlahFileIndex index = ixiptlah_index_for_path(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!index.valid) return stats;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:2261 · bloque 172
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    if (!index.valid) return stats;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2267 · bloque 173
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(path, std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) return stats;
 */
    if (!in) return stats;

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path tmp = fs::path(path.wstring() + L".rewrite");
    std::error_code ec;
    fs::remove(tmp, ec);
    {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2289 · bloque 174
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out || !ixiptlah_write_file_header(out, kIxiptlahFileVersion)) return stats;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2295 · bloque 175
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (!out || !ixiptlah_write_file_header(out, kIxiptlahFileVersion)) return stats;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2301 · bloque 176
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        for (const IxiptlahIndexedRecord& rec : index.records) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
            bool remove = false;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2313 · bloque 177
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            try { remove = drop_record && drop_record(rec.type, rec.schema); } catch (...) { remove = false; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (remove) {
 */
            if (remove) {
                ++stats.removed;
                continue;
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) {
 */
            if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                ++stats.unreadable;
                continue;
            }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            in.clear();
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:2348 · bloque 178
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            in.seekg(static_cast<std::streamoff>(rec.payload_offset), std::ios::beg);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) {
 */
            if (!in) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                ++stats.unreadable;
                continue;
            }
            std::string stored(static_cast<size_t>(rec.stored_size), '\0');
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
            if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in || !ixiptlah_write_indexed_record_raw(out, rec, stored)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2375 · bloque 179
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            if (!in || !ixiptlah_write_indexed_record_raw(out, rec, stored)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                ++stats.unreadable;
                continue;
            }
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
            ++stats.kept;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return stats;
 */
        if (!out) return stats;
    }


    in.close();

    {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2408 · bloque 180
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2414 · bloque 181
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        auto& sinks = ixiptlah_sinks();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const std::string key = path_utf8(path);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (auto it = sinks.find(key); it != sinks.end()) {
 */
        if (auto it = sinks.find(key); it != sinks.end()) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it->second && it->second->stream) {
 */
            if (it->second && it->second->stream) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2438 · bloque 182
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                it->second->stream.flush();
                it->second->stream.close();
            }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            sinks.erase(it);
        }
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2454 · bloque 183
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    fs::rename(tmp, path, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
    if (ec) {


/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2474 · bloque 184
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
        fs::copy_file(tmp, path, fs::copy_options::overwrite_existing, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec) {
 */
        if (!ec) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
            std::error_code cleanup_ec;
            fs::remove(tmp, cleanup_ec);
        } else {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            fs::remove(path, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec) {
 */
            if (!ec) {
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
                ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2518 · bloque 185
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                fs::rename(tmp, path, ec);
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) {
 */
            if (ec) {
                fs::remove(tmp, ec);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
                return stats;
            }
        }
    }
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:2536 · bloque 186
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    stats.rewritten = true;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2542 · bloque 187
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    ixiptlah_invalidate_index_cache(path);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return stats;
}

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2556 · bloque 188
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
IxiptlahCopyStats ixiptlah_append_selected_records_raw(
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& dst,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& src,
/*
 * Función ixiptlah_append_selected_records_raw: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2579 · bloque 189
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::function<bool(IxiptlahRecordType, std::uint32_t)>& accept_record) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2585 · bloque 190
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    IxiptlahCopyStats stats;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dst.empty() || src.empty()) return stats;
 */
    if (dst.empty() || src.empty()) return stats;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2597 · bloque 191
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const IxiptlahFileIndex index = ixiptlah_index_for_path(src);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!index.valid) return stats;
 */
    if (!index.valid) return stats;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.cpp:2609 · bloque 192
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    std::ifstream in(src, std::ios::binary);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) return stats;
 */
    if (!in) return stats;

/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2622 · bloque 193
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    for (const IxiptlahIndexedRecord& rec : index.records) {
        bool accepted = true;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.cpp:2629 · bloque 194
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        try { accepted = !accept_record || accept_record(rec.type, rec.schema); } catch (...) { accepted = false; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!accepted) continue;
 */
        if (!accepted) continue;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) {
 */
        if (rec.stored_size > kMaxPayloadBytes || rec.raw_size > kMaxPayloadBytes) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Ixiptlah.cpp:2647 · bloque 195
 * Este tramo pertenece a motor binario IXIPTLAH, responsable de envolturas tipadas, control de versión, compresión, índice, lectura selectiva y reescritura segura; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            ++stats.unreadable;
            continue;
        }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
        in.clear();
        in.seekg(static_cast<std::streamoff>(rec.payload_offset), std::ios::beg);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) {
 */
        if (!in) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            ++stats.unreadable;
            continue;
        }
        std::string stored(static_cast<size_t>(rec.stored_size), '\0');
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
 */
        if (rec.stored_size > 0) in.read(stored.data(), static_cast<std::streamsize>(stored.size()));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!in) {
 */
        if (!in) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            ++stats.unreadable;
            continue;
        }
        {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2698 · bloque 196
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2704 · bloque 197
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            IxiptlahSink* sink = ixiptlah_sink_for_path_locked(dst);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!sink || !sink->stream || sink->version != kIxiptlahFileVersion ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2710 · bloque 198
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            if (!sink || !sink->stream || sink->version != kIxiptlahFileVersion ||
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2716 · bloque 199
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                !ixiptlah_write_indexed_record_raw(sink->stream, rec, stored)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                ++stats.unreadable;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas.
 */
                continue;
            }
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2730 · bloque 200
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            sink->stream.flush();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!sink->stream) {
 */
            if (!sink->stream) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                ++stats.unreadable;
                continue;
            }
        }
        ++stats.copied;
        stats.target_touched = true;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (stats.target_touched) ixiptlah_invalidate_index_cache(dst);
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2754 · bloque 201
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    if (stats.target_touched) ixiptlah_invalidate_index_cache(dst);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return stats;
}

/*
 * Función ixiptlah_flush_all: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2773 · bloque 202
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_flush_all() {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2779 · bloque 203
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2785 · bloque 204
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    for (auto& [_, sink] : ixiptlah_sinks()) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (sink && sink->stream) {
 */
        if (sink && sink->stream) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2792 · bloque 205
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            sink->stream.flush();
        }
    }
}

/*
 * Función ixiptlah_close_all: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a serialización y lectura del formato binario IXIPTLAH para cargar series mensuales con menor latencia, menor memoria intermedia y control explícito de compatibilidad. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2807 · bloque 206
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_close_all() {
/*
 * Zona de concurrencia: el fragmento siguiente participa en coordinación entre hilos, trabajos diferidos o estado compartido que puede observarse desde el renderizador.
 * El bloqueo debe mantenerse corto, con propiedad clara y sin invocar funciones que puedan reentrar al mismo subsistema; de lo contrario aparecen interbloqueos o pausas intermitentes.
 * La prioridad técnica es preservar invariantes antes de publicar resultados parciales, especialmente durante descargas, materialización de teselas o preparación atmosférica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2813 · bloque 207
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    std::lock_guard<std::mutex> lock(ixiptlah_mu());
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2819 · bloque 208
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    for (auto& [_, sink] : ixiptlah_sinks()) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (sink && sink->stream) {
 */
        if (sink && sink->stream) {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.cpp:2826 · bloque 209
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            sink->stream.flush();
            sink->stream.close();
        }
    }
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el formato binario IXIPTLAH, núcleo persistente mensual que compacta datos geoespaciales y temporales para carga rápida sin reanalizar fuentes externas, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.cpp:2835 · bloque 210
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    ixiptlah_sinks().clear();
}

}
