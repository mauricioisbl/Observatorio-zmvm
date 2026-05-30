/*
 * Archivo: Writers.cpp
 * Papel técnico: escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:7 · bloque 1
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
#include "Writers.hpp"
#include "TemporalBlocks.hpp"

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:13 · bloque 2
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Función OutputStore: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:25 · bloque 3
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
OutputStore::OutputStore(fs::path root) : root_(std::move(root)), write_root_(root_) {}

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:30 · bloque 4
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
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:37 · bloque 5
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
constexpr size_t kEpidemiologyImportFlushRows = 85000;
constexpr size_t kExpectedCdmxObservationKeys = 4500000;

/*
 * Función is_epidemiology_ixiptlah_record: evalúa una condición semántica y debe mantenerse libre de efectos laterales para que pueda usarse con seguridad en filtros y decisiones repetidas.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:50 · bloque 6
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
bool is_epidemiology_ixiptlah_record(IxiptlahRecordType type, std::uint32_t schema) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:56 · bloque 7
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return schema == 1 &&
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:62 · bloque 8
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        (type == IxiptlahRecordType::EpidemiologyObservation ||
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:68 · bloque 9
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
         type == IxiptlahRecordType::MonthlyEpidemiologyBatch);
}

/*
 * Función monthly_ixiptlah_files_in_exact_dir: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:81 · bloque 10
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
std::vector<fs::path> monthly_ixiptlah_files_in_exact_dir(const fs::path& dir) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:87 · bloque 11
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    std::vector<fs::path> out;
    std::error_code ec;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!fs::exists(dir, ec) || ec) return out;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:94 · bloque 12
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!fs::exists(dir, ec) || ec) return out;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:100 · bloque 13
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (fs::directory_iterator it(dir, fs::directory_options::skip_permission_denied, ec), end; it != end && !ec; it.increment(ec)) {
        std::error_code item_ec;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it->is_regular_file(item_ec) && !item_ec && lower_ascii(path_utf8(it->path().extension())) == kIxi...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:107 · bloque 14
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        if (it->is_regular_file(item_ec) && !item_ec && lower_ascii(path_utf8(it->path().extension())) == kIxiptlahExtension) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:113 · bloque 15
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            out.push_back(it->path());
        }
    }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:121 · bloque 16
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    std::sort(out.begin(), out.end(), [](const fs::path& a, const fs::path& b) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:127 · bloque 17
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        return path_utf8(a.filename()) < path_utf8(b.filename());
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:133 · bloque 18
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    });
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:139 · bloque 19
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return out;
}

/*
 * Función replace_file_with_ready_file: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:152 · bloque 20
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
bool replace_file_with_ready_file(const fs::path& ready, const fs::path& target) {
    std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:159 · bloque 21
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    ensure_dir(target.parent_path());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:165 · bloque 22
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    fs::rename(ready, target, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec) return true;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:171 · bloque 23
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!ec) return true;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:177 · bloque 24
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:183 · bloque 25
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    fs::copy_file(ready, target, fs::copy_options::overwrite_existing, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:189 · bloque 26
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!ec) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:195 · bloque 27
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        std::error_code cleanup_ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:201 · bloque 28
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        fs::remove(ready, cleanup_ec);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:207 · bloque 29
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        return true;
    }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:214 · bloque 30
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    ec.clear();
    fs::remove(target, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) return false;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:221 · bloque 31
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (ec) return false;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:227 · bloque 32
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    ec.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:233 · bloque 33
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    fs::rename(ready, target, ec);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:239 · bloque 34
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return !ec;
}

/*
 * Función entity_code_from_text: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:252 · bloque 35
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string entity_code_from_text(const std::string& text) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:258 · bloque 36
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    const std::string n = normalize_key(text);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n == "hgo" || n.find("hidalgo") != std::string::npos || n.find(" hgo ") != std::string::npos) retu...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:264 · bloque 37
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (n == "hgo" || n.find("hidalgo") != std::string::npos || n.find(" hgo ") != std::string::npos) return "HGO";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n.find("estado de mexico") != std::string::npos || n.find("edomex") != std::string::npos) return "...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:270 · bloque 38
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (n.find("estado de mexico") != std::string::npos || n.find("edomex") != std::string::npos) return "MEX";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n == "mex") return "MEX";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:276 · bloque 39
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (n == "mex") return "MEX";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n == "cmx" || n.find("ciudad de mexico") != std::string::npos || n.find("cdmx") != std::string::np...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:282 · bloque 40
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (n == "cmx" || n.find("ciudad de mexico") != std::string::npos || n.find("cdmx") != std::string::npos || n.find("cmx") != std::string::npos) return "CMX";
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:288 · bloque 41
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return "CMX";
}

/*
 * Función entity_code_for_observation: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:301 · bloque 42
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string entity_code_for_observation(const Observation& o) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.pdf_id.size() == 3) return entity_code_from_text(" " + o.pdf_id + " ");
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:307 · bloque 43
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (o.pdf_id.size() == 3) return entity_code_from_text(" " + o.pdf_id + " ");
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:313 · bloque 44
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    return entity_code_from_text(o.pdf_file + " " + o.table_id + " " + o.pdf_id);
}

/*
 * Función observation_key: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:326 · bloque 45
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string observation_key(const Observation& o) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:332 · bloque 46
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    const std::string disease_key = o.disease_id.empty() ? normalize_key(o.disease) : o.disease_id;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:338 · bloque 47
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    const std::string jurisdiction_key = o.jurisdiction_id.empty() ? normalize_key(o.jurisdiction) : o.jurisdiction_id;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:344 · bloque 48
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return entity_code_for_observation(o) + "|" + std::to_string(o.bulletin_year) + "|" + std::to_string(o.bulletin_week) + "|" +
           std::to_string(o.page) + "|" + disease_key + "|" + jurisdiction_key + "|" +
           o.period + "|" + o.sex + "|" + std::to_string(o.value);
}

/*
 * Función quarantine_key: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:359 · bloque 49
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string quarantine_key(const QuarantineItem& q) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:365 · bloque 50
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    return q.pdf_id + "|" + std::to_string(q.page) + "|" + q.table_id + "|" +
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:371 · bloque 51
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
           q.column_key + "|" + q.reason + "|" + q.detail;
}


/*
 * Función fnv1a64: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:385 · bloque 52
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::uint64_t fnv1a64(const std::string& s) noexcept {
    std::uint64_t h = 1469598103934665603ull;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:392 · bloque 53
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (unsigned char c : s) {
        h ^= static_cast<std::uint64_t>(c);
        h *= 1099511628211ull;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:401 · bloque 54
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return h ? h : 1ull;
}

/*
 * Función observation_key_hash: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:414 · bloque 55
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::uint64_t observation_key_hash(const Observation& o) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:420 · bloque 56
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return fnv1a64(observation_key(o));
}

/*
 * Función quarantine_key_hash: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:433 · bloque 57
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::uint64_t quarantine_key_hash(const QuarantineItem& q) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:439 · bloque 58
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return fnv1a64(quarantine_key(q));
}

/*
 * Función observation_metric: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:452 · bloque 59
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string observation_metric(const Observation& o) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.period == "Sem") return "incidencia_semanal";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:458 · bloque 60
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (o.period == "Sem") return "incidencia_semanal";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.period == "SemDerivada") return "incidencia_semanal_derivada_sexo";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:464 · bloque 61
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (o.period == "SemDerivada") return "incidencia_semanal_derivada_sexo";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.period == "Acum" && o.source_year != std::to_string(o.bulletin_year)) return "acumulado_anio_ant...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:470 · bloque 62
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (o.period == "Acum" && o.source_year != std::to_string(o.bulletin_year)) return "acumulado_anio_anterior";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.period == "Acum" && (o.sex == "M" || o.sex == "F")) return "acumulado_anual_por_sexo";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:476 · bloque 63
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (o.period == "Acum" && (o.sex == "M" || o.sex == "F")) return "acumulado_anual_por_sexo";
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (o.period == "Acum") return "acumulado_anual";
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:482 · bloque 64
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (o.period == "Acum") return "acumulado_anual";
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:488 · bloque 65
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return "valor";
}

/*
 * Función year_from_source_text: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:501 · bloque 66
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
int year_from_source_text(const std::string& text) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:507 · bloque 67
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (size_t i = 0; i + 3 < text.size(); ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!std::isdigit(static_cast<unsigned char>(text[i])) ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:513 · bloque 68
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (!std::isdigit(static_cast<unsigned char>(text[i])) ||
            !std::isdigit(static_cast<unsigned char>(text[i + 1])) ||
            !std::isdigit(static_cast<unsigned char>(text[i + 2])) ||
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:516 · bloque 69
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            !std::isdigit(static_cast<unsigned char>(text[i + 3]))) continue;
        const int y = std::stoi(text.substr(i, 4));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (y >= 1800 && y <= 2300) return y;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:523 · bloque 70
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (y >= 1800 && y <= 2300) return y;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:530 · bloque 71
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return 0;
}

/*
 * Función header_value: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:543 · bloque 72
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string header_value(const std::vector<std::string>& header, const std::vector<std::string>& row, const std::string& name) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:549 · bloque 73
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (size_t i = 0; i < header.size() && i < row.size(); ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (header[i] == name) return row[i];
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:555 · bloque 74
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (header[i] == name) return row[i];
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:562 · bloque 75
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return {};
}

/*
 * Función compact_id_from_name: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:575 · bloque 76
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string compact_id_from_name(const std::string& name) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:581 · bloque 77
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    std::string key = normalize_key(name);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:587 · bloque 78
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (char& c : key) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c == ' ') c = '_';
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:593 · bloque 79
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (c == ' ') c = '_';
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:600 · bloque 80
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return key;
}

/*
 * Función normalized_hour: convierte entradas heterogéneas en claves canónicas estables, minimizando duplicados lógicos y errores por variantes textuales.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:613 · bloque 81
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
std::string normalized_hour(const std::string& hour) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (hour.size() >= 2 && std::isdigit(static_cast<unsigned char>(hour[0])) && std::isdigit(static_cast<...
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:619 · bloque 82
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (hour.size() >= 2 && std::isdigit(static_cast<unsigned char>(hour[0])) && std::isdigit(static_cast<unsigned char>(hour[1]))) return hour.substr(0, 2);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:625 · bloque 83
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    return "00";
}

/*
 * Función remove_legacy_generated_products: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:638 · bloque 84
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void remove_legacy_generated_products(const fs::path& root) {
    std::error_code ec;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!fs::exists(root, ec) || ec) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:645 · bloque 85
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!fs::exists(root, ec) || ec) return;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:651 · bloque 86
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    const std::set<std::string> legacy_names = {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:657 · bloque 87
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        "observations.csv", "observations.jsonl", "observaciones.csv", "observaciones.jsonl",
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:663 · bloque 88
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        "quarantine.csv", "tables.jsonl", "cdmx_epiatmos.sqlite.sql", "cdmx_epiatmos.sql",
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:669 · bloque 89
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        "resultados.json", "observaciones_completas.json", "DatosEpidemiologicosZMVM.json",
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:675 · bloque 90
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        "web_observaciones.jsonl", "web_observaciones_ligero.jsonl", "web_observaciones_ligero_core.tsv",
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:681 · bloque 91
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        "observaciones_en_vivo.jsonl", "observaciones_en_vivo_ligero.jsonl", "web_manifest.json",
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:687 · bloque 92
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        "semanas.json", "web_semanas.json", "web_zmvm.geojson", "HistorialEpidemiologicoCDMX_generado.csv",
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:693 · bloque 93
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        "observaciones_derivadas_sexo.jsonl", "atmosfera_archivos.json", "web_atmosfera_archivos.json"
    };
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:700 · bloque 94
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    const std::set<std::string> legacy_dirs = {"accepted", "base", "web", "progress", "quarantine", "decadas", "_last_good"};
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:706 · bloque 95
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (fs::directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec)) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) { ec.clear(); continue; }
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:712 · bloque 96
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (ec) { ec.clear(); continue; }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:718 · bloque 97
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        const std::string name = path_utf8(it->path().filename());
        std::error_code item_ec;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it->is_directory(item_ec) && !item_ec && legacy_dirs.count(name)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:725 · bloque 98
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (it->is_directory(item_ec) && !item_ec && legacy_dirs.count(name)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:731 · bloque 99
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            fs::remove_all(it->path(), ec);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:737 · bloque 100
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            ec.clear();
            continue;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (it->is_regular_file(item_ec) && !item_ec) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:745 · bloque 101
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (it->is_regular_file(item_ec) && !item_ec) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:751 · bloque 102
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            const std::string ext = lower_ascii(path_utf8(it->path().extension()));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ext == ".tsv" || ext == ".csv" || ext == ".json" || ext == ".jsonl" ||
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:757 · bloque 103
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
            if (ext == ".tsv" || ext == ".csv" || ext == ".json" || ext == ".jsonl" ||
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:763 · bloque 104
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                legacy_names.count(name) || (name.rfind("tokens_", 0) == 0 && ext == ".jsonl") ||
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:769 · bloque 105
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                (name.rfind("web_observaciones_", 0) == 0 && (ext == ".jsonl" || ext == ".json" || ext == ".tsv"))) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:775 · bloque 106
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                fs::remove(it->path(), ec);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:781 · bloque 107
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
                ec.clear();
            }
        }
    }
}

}

/*
 * Función open: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:799 · bloque 108
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::open(bool resume) {
    ensure_dir(root_);
    remove_legacy_generated_products(root_);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:807 · bloque 109
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    epidemiology_batch_buffer_.clear();
    staged_epidemiology_rebuild_ = false;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:814 · bloque 110
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    epidemiology_rebuild_root_.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:820 · bloque 111
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    write_root_ = root_;


/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:828 · bloque 112
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    observation_keys_.reserve(kExpectedCdmxObservationKeys);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!resume) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:834 · bloque 113
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!resume) {


/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:837 · bloque 114
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        staged_epidemiology_rebuild_ = true;
        epidemiology_rebuild_root_ = root_ / "_epidemiology_rebuild_tmp";
        std::error_code ec;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:840 · bloque 115
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        fs::remove_all(epidemiology_rebuild_root_, ec);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:846 · bloque 116
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        ec.clear();
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:852 · bloque 117
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        ensure_dir(epidemiology_rebuild_root_);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:858 · bloque 118
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        write_root_ = epidemiology_rebuild_root_;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:864 · bloque 119
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        for (const auto& p : temporal_tsv_files(root_)) fs::remove(p, ec);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:870 · bloque 120
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        for (const auto& p : temporal_csv_files(root_)) fs::remove(p, ec);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:876 · bloque 121
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        for (const auto& p : temporal_json_files(root_)) fs::remove(p, ec);
    } else {
        load_existing_observations();
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:879 · bloque 122
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        load_existing_quarantine();
    }
    opened_ = true;
}

/*
 * Función load_existing_observations: ingiere, localiza o interpreta datos de entrada y debe fallar de forma contenida ante archivos incompletos, rutas ausentes o formatos parciales.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:894 · bloque 123
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::load_existing_observations() {
    observation_count_ = 0;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:901 · bloque 124
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    observation_keys_.clear();
/*
 * Función temporal_read_epidemiology_records: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:912 · bloque 125
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    temporal_read_epidemiology_records(root_, [&](const TemporalEpidemiologyRecord& r) {
        Observation o;
        o.pdf_id = r.entity;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:920 · bloque 126
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.bulletin_year = r.year;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:926 · bloque 127
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.bulletin_week = r.epi_week;
        o.page = r.page;
        o.disease = r.disease;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:929 · bloque 128
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.disease_id = compact_id_from_name(o.disease);
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:935 · bloque 129
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.cie10 = r.cie10;
        o.jurisdiction = r.jurisdiction;
        o.jurisdiction_id = compact_id_from_name(o.jurisdiction);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:943 · bloque 130
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.source_year = std::to_string(o.bulletin_year);
        o.period = r.period;
        o.sex = r.sex;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:946 · bloque 131
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        o.value = r.value;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:952 · bloque 132
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        observation_keys_.insert(observation_key_hash(o));
        ++observation_count_;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:959 · bloque 133
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        return true;
    });
}

/*
 * Función load_existing_quarantine: ingiere, localiza o interpreta datos de entrada y debe fallar de forma contenida ante archivos incompletos, rutas ausentes o formatos parciales.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:973 · bloque 134
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::load_existing_quarantine() {
    quarantine_count_ = 0;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:980 · bloque 135
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    quarantine_keys_.clear();
}

/*
 * Función write_observation: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:993 · bloque 136
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_observation(const Observation& o) {
    const std::uint64_t key = observation_key_hash(o);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (observation_keys_.find(key) != observation_keys_.end()) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1000 · bloque 137
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (observation_keys_.find(key) != observation_keys_.end()) return;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1006 · bloque 138
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    TemporalEpidemiologyRecord r;
    r.entity = entity_code_for_observation(o);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1013 · bloque 139
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    r.year = o.bulletin_year;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1019 · bloque 140
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    r.epi_week = o.bulletin_week;
    r.page = o.page;
    r.disease = o.disease;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1022 · bloque 141
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    r.cie10 = o.cie10;
    r.jurisdiction = o.jurisdiction;
    r.period = o.period;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1025 · bloque 142
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    r.sex = o.sex.empty() ? (o.period == "Sem" ? "total" : o.sex) : o.sex;
    r.value = o.value;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1032 · bloque 143
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    epidemiology_batch_buffer_.push_back(std::move(r));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (epidemiology_batch_buffer_.size() >= kEpidemiologyImportFlushRows) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1038 · bloque 144
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (epidemiology_batch_buffer_.size() >= kEpidemiologyImportFlushRows) {
        flush_epidemiology_batch_buffer();
    }
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1046 · bloque 145
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    observation_keys_.insert(key);
    ++observation_count_;
}

/*
 * Función write_quarantine: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1060 · bloque 146
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_quarantine(const QuarantineItem& q) {
    const std::uint64_t key = quarantine_key_hash(q);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (quarantine_keys_.find(key) != quarantine_keys_.end()) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1067 · bloque 147
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (quarantine_keys_.find(key) != quarantine_keys_.end()) return;


/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1075 · bloque 148
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    quarantine_keys_.insert(key);
    ++quarantine_count_;
}

/*
 * Función write_tokens_jsonl: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1089 · bloque 149
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::write_tokens_jsonl(const PdfDocument&) {

}

/*
 * Función append_table: modifica estado acumulado y debe hacerlo preservando invariantes, atomicidad lógica y consistencia de las estructuras relacionadas.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1103 · bloque 150
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::append_table(const TableCandidate& t) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!opened_) throw std::runtime_error("OutputStore no abierto.");
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1109 · bloque 151
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!opened_) throw std::runtime_error("OutputStore no abierto.");
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1115 · bloque 152
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    epidemiology_batch_buffer_.reserve(std::min<size_t>(
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1121 · bloque 153
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        kEpidemiologyImportFlushRows + t.accepted.size(), kEpidemiologyImportFlushRows * 2));
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1127 · bloque 154
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (const auto& o : t.accepted) {
        const std::uint64_t key = observation_key_hash(o);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (observation_keys_.find(key) != observation_keys_.end()) continue;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1134 · bloque 155
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (observation_keys_.find(key) != observation_keys_.end()) continue;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1140 · bloque 156
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        TemporalEpidemiologyRecord r;
        r.entity = entity_code_for_observation(o);
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1147 · bloque 157
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        r.year = o.bulletin_year;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1153 · bloque 158
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        r.epi_week = o.bulletin_week;
        r.page = o.page;
        r.disease = o.disease;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1156 · bloque 159
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        r.cie10 = o.cie10;
        r.jurisdiction = o.jurisdiction;
        r.period = o.period;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1159 · bloque 160
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        r.sex = o.sex.empty() ? (o.period == "Sem" ? "total" : o.sex) : o.sex;
        r.value = o.value;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1166 · bloque 161
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        epidemiology_batch_buffer_.push_back(std::move(r));
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1172 · bloque 162
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        observation_keys_.insert(key);
        ++observation_count_;
    }


/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (epidemiology_batch_buffer_.size() >= kEpidemiologyImportFlushRows) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1182 · bloque 163
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (epidemiology_batch_buffer_.size() >= kEpidemiologyImportFlushRows) {
        flush_epidemiology_batch_buffer();
    }
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1190 · bloque 164
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (const auto& q : t.quarantine) write_quarantine(q);
    const std::string force_flush = getenv_utf8_or_empty("TLALPOWA_FLUSH_EVERY_TABLE");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!force_flush.empty() && force_flush != "0") {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1197 · bloque 165
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    if (!force_flush.empty() && force_flush != "0") {
        flush_epidemiology_batch_buffer();
        temporal_flush_append_streams();
    }
}

/*
 * Función flush_epidemiology_batch_buffer: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1213 · bloque 166
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::flush_epidemiology_batch_buffer() {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (epidemiology_batch_buffer_.empty()) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1219 · bloque 167
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (epidemiology_batch_buffer_.empty()) return;


/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1227 · bloque 168
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::vector<TemporalEpidemiologyRecord> batch;
    batch.swap(epidemiology_batch_buffer_);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (staged_epidemiology_rebuild_) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1234 · bloque 169
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (staged_epidemiology_rebuild_) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1240 · bloque 170
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        temporal_append_epidemiology_records_batch_exact_root(write_root_, batch);
    } else {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1247 · bloque 171
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        temporal_append_epidemiology_records_batch(root_, batch);
    }
}

/*
 * Función commit_epidemiology_rebuild: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1261 · bloque 172
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::commit_epidemiology_rebuild() {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!staged_epidemiology_rebuild_ || epidemiology_rebuild_root_.empty()) return;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1267 · bloque 173
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    if (!staged_epidemiology_rebuild_ || epidemiology_rebuild_root_.empty()) return;


/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1270 · bloque 174
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    temporal_close_append_streams();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1276 · bloque 175
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
    const std::vector<fs::path> staged_files = monthly_ixiptlah_files_in_exact_dir(epidemiology_rebuild_root_);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1282 · bloque 176
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    for (const fs::path& staged_file : staged_files) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1288 · bloque 177
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        const fs::path target = root_ / staged_file.filename();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1294 · bloque 178
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        const fs::path ready = fs::path(target.wstring() + L".epi_ready");
        std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1301 · bloque 179
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        fs::remove(ready, ec);
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1307 · bloque 180
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        ec.clear();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fs::exists(target, ec) && !ec && file_size_or_zero(target) > 0) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1313 · bloque 181
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (fs::exists(target, ec) && !ec && file_size_or_zero(target) > 0) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1319 · bloque 182
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            fs::copy_file(target, ready, fs::copy_options::overwrite_existing, ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (ec) throw std::runtime_error("No se pudo preparar fusion ixiptlah: " + path_utf8(target));
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1325 · bloque 183
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            if (ec) throw std::runtime_error("No se pudo preparar fusion ixiptlah: " + path_utf8(target));
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1331 · bloque 184
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            const IxiptlahRewriteStats purge = ixiptlah_rewrite_without_records(ready, is_epidemiology_ixiptlah_record);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (purge.removed > 0 && !purge.rewritten) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1337 · bloque 185
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
            if (purge.removed > 0 && !purge.rewritten) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1343 · bloque 186
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
                throw std::runtime_error("No se pudo limpiar epidemiologia en fusion ixiptlah: " + path_utf8(target));
            }
        }
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1351 · bloque 187
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        const IxiptlahCopyStats copied = ixiptlah_append_selected_records_raw(ready, staged_file, is_epidemiology_ixiptlah_record);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (copied.copied == 0 || copied.unreadable > 0 || !copied.target_touched) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1357 · bloque 188
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (copied.copied == 0 || copied.unreadable > 0 || !copied.target_touched) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1363 · bloque 189
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            throw std::runtime_error("No se pudo copiar el nucleo epidemiologico nuevo hacia ixiptlah: " + path_utf8(target));
        }
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1370 · bloque 190
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
        ixiptlah_close_all();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!replace_file_with_ready_file(ready, target)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1376 · bloque 191
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
        if (!replace_file_with_ready_file(ready, target)) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · arquitectura IXIPTLAH mensual · Writers.cpp:1382 · bloque 192
 * IXIPTLAH no debe interpretarse como una extensión por categoría, sino como un contenedor mensual heterogéneo: el archivo de un mes concentra múltiples núcleos internos, cada uno identificado por tipo de registro, versión de esquema, tamaño bruto, tamaño almacenado y códec.
 * Esa envoltura de registros es la defensa contra corrupción cruzada: si un núcleo epidemiológico, atmosférico o de movilidad queda dañado o debe regenerarse, la operación correcta es reescribir, purgar o copiar selectivamente ese núcleo sin destruir los demás tipos de información presentes en el mismo mes.
 * Nunca crear una familia paralela de IXIPTLAH por cada tipo de información; esa fragmentación rompería la premisa temporal del sistema, duplicaría índices, degradaría el cacheo y haría más difícil razonar sobre integridad transdominio.
 * La lectura selectiva debe tratar el archivo como secuencia de cápsulas autónomas; saltar un tipo no solicitado no equivale a ignorar el archivo, sino a mantener encapsulado el núcleo que no participa en la consulta actual.
 * La escritura debe preservar atomía observable: publicar completos los registros válidos, invalidar índices después de mutar el archivo y evitar estados intermedios en los que una sesión gráfica lea una mezcla de generaciones incompatibles.
 */
            throw std::runtime_error("No se pudo publicar fusion ixiptlah: " + path_utf8(target));
        }
    }
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1385 · bloque 193
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    std::error_code ec;
    fs::remove_all(epidemiology_rebuild_root_, ec);
    staged_epidemiology_rebuild_ = false;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1393 · bloque 194
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    epidemiology_rebuild_root_.clear();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1399 · bloque 195
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    write_root_ = root_;
}

/*
 * Función flush_streams: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1412 · bloque 196
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::flush_streams() {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1418 · bloque 197
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    temporal_flush_append_streams_if_due(1800);
}

/*
 * Función flush_live_outputs: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1431 · bloque 198
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::flush_live_outputs(const fs::path&, bool) {
    flush_epidemiology_batch_buffer();
    flush_streams();
}

/*
 * Función write_derived_outputs: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1446 · bloque 199
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_derived_outputs(const fs::path&) {
    flush_epidemiology_batch_buffer();
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1453 · bloque 200
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    temporal_rebuild_all_json_indexes(root_);
}

/*
 * Función finalize: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1466 · bloque 201
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::finalize() {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1472 · bloque 202
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    flush_epidemiology_batch_buffer();
    commit_epidemiology_rebuild();
    temporal_flush_append_streams();
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1480 · bloque 203
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
    temporal_rebuild_all_json_indexes(root_);
    temporal_close_append_streams();
}

/*
 * Función write_obs_csv_header: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1494 · bloque 204
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::write_obs_csv_header() {}
/*
 * Función write_quarantine_csv_header: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1505 · bloque 205
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::write_quarantine_csv_header() {}
/*
 * Función write_sql_header: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1516 · bloque 206
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_sql_header() {}
/*
 * Función write_results_json: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1527 · bloque 207
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_results_json() const {}
/*
 * Función write_master_csv: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Writers.cpp:1538 · bloque 208
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
void OutputStore::write_master_csv() const {}
/*
 * Función write_derived_sex_incidence: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1549 · bloque 209
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_derived_sex_incidence() const {}
/*
 * Función write_atmospheric_inventory: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a escritores de artefactos: TSV, JSON, reportes y archivos de salida que deben ser idempotentes y trazables. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En la escritura de salidas analíticas, reportes y datos exportados, donde la robustez exige no mezclar formato de presentación con transformación semántica, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Writers.cpp:1560 · bloque 210
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
void OutputStore::write_atmospheric_inventory() const {}

}
