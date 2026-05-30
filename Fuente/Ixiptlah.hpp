/*
 * Archivo: Ixiptlah.hpp
 * Papel técnico: especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#pragma once
#include "TextUtils.hpp"

#include <functional>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:15 · bloque 1
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:22 · bloque 2
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
constexpr const char* kIxiptlahExtension = ".ixiptlah";

/*
 * Enumeración IxiptlahRecordType: codifica estados discretos usados por especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * Los valores deben tratarse como contrato estable entre ramas de control, persistencia y UI; renombrar o reordenar sin revisar consumidores puede romper filtros, serialización, selección visual o compatibilidad binaria.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:33 · bloque 3
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
enum class IxiptlahRecordType : std::uint32_t {
    EpidemiologyObservation = 1,
    EpidemiologyQuarantine = 2,
    AtmosphereMeasurement = 10,
    AtmosphereTerritoryAverage = 11,
    AtmosphereRenderSummary = 12,
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Ixiptlah.hpp:39 · bloque 4
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    ProcessedPdf = 20,
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Ixiptlah.hpp:45 · bloque 5
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    ProcessedPage = 21,
    RunState = 30,
    LivePreview = 31,


/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:55 · bloque 6
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    MonthlyDictionary = 100,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:61 · bloque 7
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    MonthlyEpidemiologyBatch = 101,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:67 · bloque 8
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    MonthlyAtmosphereRenderBatch = 102,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:73 · bloque 9
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    MonthlyAtmosphereMeasurementBatch = 103,
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:79 · bloque 10
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    MonthlyAtmosphereTerritoryBatch = 104,


/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Ixiptlah.hpp:87 · bloque 11
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    MonthlySourceInventory = 105
};

/*
 * Declaración ixiptlah_path: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:99 · bloque 12
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
fs::path ixiptlah_path(const fs::path& root, const std::string& stem);


/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
template <typename T>
/*
 * Función ixiptlah_write_value: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:118 · bloque 13
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_value(std::ostream& out, const T& value) {
/*
 * Declaración write: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    out.write(reinterpret_cast<const char*>(&value), static_cast<std::streamsize>(sizeof(T)));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return static_cast<bool>(out);
}

/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
template <typename T>
/*
 * Función ixiptlah_read_value: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:153 · bloque 14
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_read_value(std::istream& in, T& value) {
/*
 * Declaración read: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    in.read(reinterpret_cast<char*>(&value), static_cast<std::streamsize>(sizeof(T)));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return static_cast<bool>(in);
}

/*
 * Declaración ixiptlah_write_string: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:181 · bloque 15
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_write_string(std::ostream& out, const std::string& value);
/*
 * Declaración ixiptlah_read_string: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:191 · bloque 16
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool ixiptlah_read_string(std::istream& in, std::string& value);

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:198 · bloque 17
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
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:204 · bloque 18
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
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.hpp:210 · bloque 19
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
                           std::uint32_t schema_version,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                           const std::function<bool(std::ostream&)>& write_payload);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:226 · bloque 20
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
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:232 · bloque 21
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
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Ixiptlah.hpp:238 · bloque 22
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
                                        std::uint32_t schema_version,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
                                        const std::function<bool(std::ostream&)>& write_payload);

/*
 * Tipo IxiptlahRewriteStats: agrupa estado o registros usados por especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:259 · bloque 23
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahRewriteStats {
    std::uint64_t kept = 0;
    std::uint64_t removed = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::uint64_t unreadable = 0;
    bool rewritten = false;
};

/*
 * Tipo IxiptlahCopyStats: agrupa estado o registros usados por especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:280 · bloque 24
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
struct IxiptlahCopyStats {
    std::uint64_t copied = 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    std::uint64_t unreadable = 0;
    bool target_touched = false;
};

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:296 · bloque 25
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
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& path,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:312 · bloque 26
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::function<bool(IxiptlahRecordType, std::uint32_t)>& drop_record);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:318 · bloque 27
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
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& dst,
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path& src,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:340 · bloque 28
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::function<bool(IxiptlahRecordType, std::uint32_t)>& accept_record);

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:347 · bloque 29
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_read_records(const fs::path& path,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:357 · bloque 30
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                          const std::function<bool(IxiptlahRecordType, std::uint32_t, std::istream&)>& read_payload);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:363 · bloque 31
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
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:369 · bloque 32
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                                   const std::function<bool(IxiptlahRecordType, std::uint32_t)>& accept_record,
/*
 * Declaración bool: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:379 · bloque 33
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                                   const std::function<bool(IxiptlahRecordType, std::uint32_t, std::istream&)>& read_payload);

/*
 * Declaración ixiptlah_flush_all: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:390 · bloque 34
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_flush_all();
/*
 * Declaración ixiptlah_close_all: expone una operación de especificación pública del contenedor IXIPTLAH, sus encabezados, bloques, índices y funciones de lectura/escritura.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el contrato del núcleo IXIPTLAH, con tipos suficientemente explícitos para evitar ambigüedades de endianness, versión, rangos y propiedad de buffers, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Ixiptlah.hpp:400 · bloque 35
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
void ixiptlah_close_all();

}
