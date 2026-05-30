

/*
 * Archivo: Dashboard.cpp
 * Papel técnico: salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#include "Dashboard.hpp"
#include <cmath>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Dashboard.cpp:15 · bloque 1
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {


/*
 * Función ensure: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
void Dashboard::ensure() {


    ensure_dir(root_);
}


/*
 * Función color_for: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
std::string Dashboard::color_for(const std::string& s, int alpha) {
    const uint64_t h = std::stoull(simple_hash_hex(s).substr(0, 12), nullptr, 16);
    const int r = 70 + static_cast<int>((h >> 0) & 0x7f);
    const int g = 70 + static_cast<int>((h >> 8) & 0x7f);
    const int b = 70 + static_cast<int>((h >> 16) & 0x7f);
    std::ostringstream os;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    os << "rgba(" << r << ',' << g << ',' << b << ',' << std::fixed << std::setprecision(2) << (alpha / 255.0) << ')';
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return os.str();
}


/*
 * Función rel_img_path: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:76 · bloque 2
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
std::string Dashboard::rel_img_path(const fs::path& image, const fs::path& html) {
    std::error_code ec;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    auto rel = fs::relative(image, html.parent_path(), ec);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!ec) return path_utf8(rel.generic_string());
 */
    if (!ec) return path_utf8(rel.generic_string());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    return path_utf8(image.generic_string());
}

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · Dashboard.cpp:101 · bloque 3
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace {


/*
 * Función dashboard_rect_has_area: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
bool dashboard_rect_has_area(const Rect& r) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return r.width() > 0.01 && r.height() > 0.01;
}


/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
std::string dashboard_role_color(const ColumnBand& c, int alpha = 218) {
    int r = 92, g = 106, b = 120;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c.period == "Sem") { r = 255; g = 205; b = 0; }
 */
    if (c.period == "Sem") { r = 255; g = 205; b = 0; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: else if (c.sex == "M") { r = 0; g = 111; b = 205; }
 */
    else if (c.sex == "M") { r = 0; g = 111; b = 205; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: else if (c.sex == "F") { r = 225; g = 36; b = 139; }
 */
    else if (c.sex == "F") { r = 225; g = 36; b = 139; }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: else if (c.period == "Acum") { r = 0; g = 151; b = 92; }
 */
    else if (c.period == "Acum") { r = 0; g = 151; b = 92; }
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:155 · bloque 4
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::ostringstream os;
    os << "rgba(" << r << ',' << g << ',' << b << ','
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
       << std::fixed << std::setprecision(2) << (alpha / 255.0) << ')';
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return os.str();
}

/*
 * Función dashboard_rect_key: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
std::string dashboard_rect_key(const Rect& r, const char* tag) {
    std::ostringstream os;
    os << tag << ':' << static_cast<int>(std::round(r.x0))
       << ':' << static_cast<int>(std::round(r.y0))
       << ':' << static_cast<int>(std::round(r.x1))
       << ':' << static_cast<int>(std::round(r.y1));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return os.str();
}

}


/*
 * Función push_page: modifica estado acumulado y debe hacerlo preservando invariantes, atomicidad lógica y consistencia de las estructuras relacionadas.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
void Dashboard::push_page(DashboardPage page, const PipelineStats& stats) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
    ensure();
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    last_.push_back(std::move(page));
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:226 · bloque 5
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    while (last_.size() > 2) last_.erase(last_.begin());
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    write(stats);
}


/*
 * Función write: materializa salida persistente o binaria, por lo que debe preservar orden, codificación, idempotencia y compatibilidad hacia adelante.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
void Dashboard::write(const PipelineStats& stats) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    const fs::path html = index_path();
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    ensure_dir(html.parent_path());
    std::ostringstream os;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · frontera de descarga y E/S importadora · Dashboard.cpp:265 · bloque 6
 * La E/S de importación cruza una frontera no determinista: red, disco, antivirus, bloqueos del sistema, codificación oficial irregular y servidores que cambian encabezados sin cambiar contenido semántico.
 * Por eso este punto debe distinguir con precisión ausencia, duplicado, archivo vacío, descarga truncada, respuesta HTTP aceptable pero semánticamente inútil, y recurso legible que todavía no alcanza el umbral de materialización.
 * En los importadores de sopa de letras, una descarga exitosa no es un dato aceptado; sólo es materia prima que debe pasar por normalización, control de tamaño, extracción, validación y finalmente conversión al núcleo IXIPTLAH mensual pertinente.
 * No introducir rutas laterales de persistencia por comodidad diagnóstica: los artefactos auxiliares pueden existir como caché o bitácora, pero el resultado analítico oficial debe converger a IXIPTLAH mensual centralizado.
 * La interfaz debe recibir progreso, cancelación y mensajes de degradación sin depender de que el archivo final ya esté completo; esto evita congelamientos y permite reintentar sin duplicar registros aceptados.
 */
    os << "<!doctype html><html><head><meta charset='utf-8'><meta http-equiv='refresh' content='2'>"
       << "<title>Tlalpowa</title>"
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
       << "<style>body{margin:0;background:#111;color:#eee;font-family:Segoe UI,Arial,sans-serif;}"
       << ".top{position:sticky;top:0;background:#181818;padding:12px 18px;border-bottom:1px solid #333;z-index:10}"
/*
 * Función stats: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a salida textual y tabular de diagnóstico para revisar datasets, jurisdicciones, enfermedades y productos generados fuera del modo gráfico. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
       << ".stats{display:grid;grid-template-columns:repeat(7,auto);gap:12px;font-size:13px}.card{background:#1c1c1c;border:1px solid #333;border-radius:10px;margin:12px;padding:12px}"
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
       << ".grid{display:grid;grid-template-columns:1fr 1fr;gap:12px}.pagewrap{position:relative;width:100%;overflow:auto;background:#222;border-radius:8px}.pagewrap img{width:100%;display:block}"
       << ".caption{font-size:13px;color:#ddd;margin-top:8px;word-break:break-all}.legend{font-size:12px;color:#ccc;margin-top:8px}.warn{color:#ffd18a} svg{position:absolute;inset:0;width:100%;height:100%;pointer-events:none}"
       << "</style></head><body>";
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:293 · bloque 7
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    os << "<div class='top'><b>Tlalpowa</b><div class='stats'>"
       << "<span>PDF " << stats.pdf_done << "/" << stats.pdf_total << "</span>"
       << "<span>Páginas " << stats.pages_done << "/" << stats.pages_total << "</span>"
       << "<span>Con tablas " << stats.pages_with_tables << "</span>"
       << "<span>Tablas " << stats.tables_detected << "</span>"
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
       << "<span>Aceptadas " << stats.observations_accepted << "</span>"
       << "<span>Cuarentena " << stats.quarantine_items << "</span>"
       << "<span>UTC " << now_utc_iso() << "</span></div></div>";
    os << "<div class='grid'>";
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (const auto& p : last_) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Dashboard.cpp:318 · bloque 8
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
        os << "<div class='card'><div class='pagewrap'><img src='" << json_escape(rel_img_path(p.image_path, html)) << "'>";
        os << "<svg viewBox='0 0 " << p.page_width << ' ' << p.page_height << "' preserveAspectRatio='none'>";
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (const auto& t : p.tables) {
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
            const double table_y0 = t.rows.empty() ? 0.0 : t.rows.front().y0;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:337 · bloque 9
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            const double table_y1 = t.rows.empty() ? p.page_height : t.rows.back().y1;

/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
            for (const auto& r : t.rows) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
                os << "<line x1='0' y1='" << r.y_mid << "' x2='" << p.page_width << "' y2='" << r.y_mid
                   << "' stroke='rgba(226,92,0,.50)' stroke-width='1.1'/>";
            }

/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
            std::set<std::string> underlined;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
            for (const auto& c : t.columns) {
                double y0 = table_y0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dashboard_rect_has_area(c.header_box)) y0 = std::min(y0, c.header_box.y0);
 */
                if (dashboard_rect_has_area(c.header_box)) y0 = std::min(y0, c.header_box.y0);
                os << "<line x1='" << c.x_mid << "' y1='" << y0 << "' x2='" << c.x_mid << "' y2='" << table_y1
                   << "' stroke='" << dashboard_role_color(c) << "' stroke-width='2.1'/>";

/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dashboard_rect_has_area(c.header_box)) {
 */
                if (dashboard_rect_has_area(c.header_box)) {
                    os << "<line x1='" << c.header_box.x0 << "' y1='" << c.header_box.y1 + 2.0
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
                       << "' x2='" << c.header_box.x1 << "' y2='" << c.header_box.y1 + 2.0
                       << "' stroke='" << dashboard_role_color(c, 190) << "' stroke-width='1.3'/>";
                }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dashboard_rect_has_area(c.disease_box)) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · Dashboard.cpp:396 · bloque 10
 * Este tramo pertenece a panel analítico, que consume datos agregados sin alterar la semántica persistida por los importadores; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                if (dashboard_rect_has_area(c.disease_box)) {
                    const std::string key = dashboard_rect_key(c.disease_box, "disease");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (underlined.insert(key).second) {
 */
                    if (underlined.insert(key).second) {
                        os << "<line x1='" << c.disease_box.x0 << "' y1='" << c.disease_box.y1 + 2.0
                           << "' x2='" << c.disease_box.x1 << "' y2='" << c.disease_box.y1 + 2.0
                           << "' stroke='rgba(30,42,54,.82)' stroke-width='1.5'/>";
                    }
                }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dashboard_rect_has_area(c.cie10_box)) {
 */
                if (dashboard_rect_has_area(c.cie10_box)) {
                    const std::string key = dashboard_rect_key(c.cie10_box, "cie10");
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (underlined.insert(key).second) {
 */
                    if (underlined.insert(key).second) {
                        os << "<line x1='" << c.cie10_box.x0 << "' y1='" << c.cie10_box.y1 + 2.0
                           << "' x2='" << c.cie10_box.x1 << "' y2='" << c.cie10_box.y1 + 2.0
                           << "' stroke='rgba(30,42,54,.74)' stroke-width='1.3'/>";
                    }
                }
            }
            os << "<rect x='1' y='1' width='" << std::max(1.0, p.page_width-2) << "' height='" << std::max(1.0, p.page_height-2) << "' fill='none' stroke='" << color_for(t.table_id, 210) << "' stroke-width='1'/>";
        }
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
        os << "</svg></div>";
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · Dashboard.cpp:441 · bloque 11
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
        os << "<div class='caption'><b>" << json_escape(p.pdf_file) << "</b><br>Página " << p.page << "</div>";
        os << "<div class='legend'>";
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (const auto& t : p.tables) {
            os << "<div><span style='display:inline-block;width:12px;height:12px;background:" << color_for(t.table_id, 120) << ";border-radius:3px'></span> "
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · Dashboard.cpp:455 · bloque 12
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
               << json_escape(t.table_id) << " · filas " << t.rows.size() << " · columnas " << t.columns.size()
               << " · aceptadas " << t.accepted.size() << " · cuarentena " << t.quarantine.size() << "</div>";
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p.tables.empty()) os << "<span class='warn'>Sin tabla epidemiológica reconstruible en esta página....
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · Dashboard.cpp:463 · bloque 13
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
        if (p.tables.empty()) os << "<span class='warn'>Sin tabla epidemiológica reconstruible en esta página.</span>";
        os << "</div></div>";
    }
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales.
 */
    os << "</div></body></html>";
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el panel resumen y las vistas agregadas, responsable de condensar métricas sin modificar los datos subyacentes ni reordenar estados globales, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    write_text_file(html, os.str());
}

}
