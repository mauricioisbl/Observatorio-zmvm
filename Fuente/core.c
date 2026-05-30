

/*
 * Archivo: core.c
 * Papel técnico: núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#include "core.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


/*
 * Función oz_is_space: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_is_space(char c) {
    unsigned char u = (unsigned char)c;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return u == ' ' || u == '\t' || u == '\n' || u == '\r' || u == '\f' || u == '\v';
}

/*
 * Función oz_is_digit: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_is_digit(char c) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return c >= '0' && c <= '9';
}

/*
 * Función oz_key_at: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_key_at(const char* s, size_t n, size_t p, const char* key, size_t key_len) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p == 0 || p + key_len >= n) return 0;
 */
    if (p == 0 || p + key_len >= n) return 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return s[p - 1] == '"' && s[p + key_len] == '"' && memcmp(s + p, key, key_len) == 0;
}


/*
 * Función oz_find_key_value_start: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_find_key_value_start(const char* s, size_t n, const char* key, size_t* out) {
    const size_t key_len = key ? strlen(key) : 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !key || key_len == 0 || key_len + 3 > n) return 0;
 */
    if (!s || !key || key_len == 0 || key_len + 3 > n) return 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = 1; p + key_len < n; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[p] != key[0] || !oz_key_at(s, n, p, key, key_len)) continue;
 */
        if (s[p] != key[0] || !oz_key_at(s, n, p, key, key_len)) continue;
        size_t q = p + key_len + 1;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (q < n && oz_is_space(s[q])) ++q;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (q >= n || s[q] != ':') continue;
 */
        if (q >= n || s[q] != ':') continue;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:125 · bloque 1
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        ++q;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (q < n && oz_is_space(s[q])) ++q;
        *out = q;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return 1;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}


/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:154 · bloque 2
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
int ozmvm_json_string_span(const char* s, size_t n, const char* key,
/*
 * Función ozmvm_json_string_span: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                           size_t* out_begin, size_t* out_end, int* out_has_escape) {
    size_t p = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out_begin || !out_end || !oz_find_key_value_start(s, n, key, &p)) return 0;
 */
    if (!out_begin || !out_end || !oz_find_key_value_start(s, n, key, &p)) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p >= n || s[p] != '"') return 0;
 */
    if (p >= n || s[p] != '"') return 0;
    ++p;
    const size_t begin = p;
    int escaped = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (; p < n; ++p) {
        const char c = s[p];
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c == '"') {
 */
        if (c == '"') {
            *out_begin = begin;
            *out_end = p;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_has_escape) *out_has_escape = escaped;
 */
            if (out_has_escape) *out_has_escape = escaped;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return 1;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (c == '\\' && p + 1 < n) {
 */
        if (c == '\\' && p + 1 < n) {
            escaped = 1;
            ++p;
        }
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}


/*
 * Función ozmvm_json_i64: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:234 · bloque 3
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
int64_t ozmvm_json_i64(const char* s, size_t n, const char* key) {
    size_t p = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_find_key_value_start(s, n, key, &p)) return 0;
 */
    if (!oz_find_key_value_start(s, n, key, &p)) return 0;
    int neg = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p < n && s[p] == '-') {
 */
    if (p < n && s[p] == '-') {
        neg = 1;
        ++p;
    }
    int64_t v = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    int seen = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (p < n && oz_is_digit(s[p])) {
        seen = 1;
        const int d = s[p++] - '0';
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v <= (INT64_MAX - d) / 10) v = v * 10 + d;
 */
        if (v <= (INT64_MAX - d) / 10) v = v * 10 + d;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!seen) return 0;
 */
    if (!seen) return 0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return neg ? -v : v;
}


/*
 * Función ozmvm_attr_double_span: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
int ozmvm_attr_double_span(const char* s, size_t begin, size_t end, const char* attr, double* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !attr || !out || begin >= end) return 0;
 */
    if (!s || !attr || !out || begin >= end) return 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    const size_t attr_len = strlen(attr);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = begin; p + attr_len + 2 < end; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[p] != attr[0] || memcmp(s + p, attr, attr_len) != 0) continue;
 */
        if (s[p] != attr[0] || memcmp(s + p, attr, attr_len) != 0) continue;
        size_t q = p + attr_len;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (q + 1 >= end || s[q] != '=' || s[q + 1] != '"') continue;
 */
        if (q + 1 >= end || s[q] != '=' || s[q + 1] != '"') continue;
        q += 2;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:332 · bloque 4
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        int neg = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (q < end && s[q] == '-') {
 */
        if (q < end && s[q] == '-') {
            neg = 1;
            ++q;
        }
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        double value = 0.0;
        int seen = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (q < end && oz_is_digit(s[q])) {
            seen = 1;
            value = value * 10.0 + (double)(s[q++] - '0');
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (q < end && s[q] == '.') {
 */
        if (q < end && s[q] == '.') {
            ++q;
            double base = 0.1;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
            while (q < end && oz_is_digit(s[q])) {
                seen = 1;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
                value += (double)(s[q++] - '0') * base;
                base *= 0.1;
            }
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!seen) return 0;
 */
        if (!seen) return 0;
        *out = neg ? -value : value;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return 1;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}

/*
 * Función ozmvm_first_year_20xx: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
int ozmvm_first_year_20xx(const char* s, size_t n) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || n < 4) return 0;
 */
    if (!s || n < 4) return 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i + 3 < n; ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[i] == '2' && s[i + 1] == '0' && oz_is_digit(s[i + 2]) && oz_is_digit(s[i + 3])) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:432 · bloque 5
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (s[i] == '2' && s[i + 1] == '0' && oz_is_digit(s[i + 2]) && oz_is_digit(s[i + 3])) {
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return 2000 + (s[i + 2] - '0') * 10 + (s[i + 3] - '0');
        }
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}

/*
 * Función ozmvm_week_after_marker: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
int ozmvm_week_after_marker(const char* s, size_t n, const char* marker) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !marker || !*marker) return 0;
 */
    if (!s || !marker || !*marker) return 0;
    const size_t ml = strlen(marker);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n < ml + 1) return 0;
 */
    if (n < ml + 1) return 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = 0; p + ml < n; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[p] != marker[0] || memcmp(s + p, marker, ml) != 0) continue;
 */
        if (s[p] != marker[0] || memcmp(s + p, marker, ml) != 0) continue;
        size_t i = p + ml;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (i < n && oz_is_space(s[i])) ++i;
        int v = 0, digits = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (i < n && oz_is_digit(s[i]) && digits < 2) {
            v = v * 10 + (s[i++] - '0');
            ++digits;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (digits > 0 && v >= 1 && v <= 53) return v;
 */
        if (digits > 0 && v >= 1 && v <= 53) return v;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}


/*
 * Función ozmvm_week_before_del_year: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
int ozmvm_week_before_del_year(const char* s, size_t n) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s) return 0;
 */
    if (!s) return 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_is_digit(s[i])) continue;
 */
        if (!oz_is_digit(s[i])) continue;
        int v = 0, digits = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:548 · bloque 6
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        size_t j = i;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (j < n && oz_is_digit(s[j]) && digits < 2) {
            v = v * 10 + (s[j++] - '0');
            ++digits;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (digits == 0 || v < 1 || v > 53) {
 */
        if (digits == 0 || v < 1 || v > 53) {
            i = j;
            continue;
        }
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        while (j < n && oz_is_space(s[j])) ++j;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (j + 7 <= n && memcmp(s + j, "del ", 4) == 0) j += 4;
 */
        if (j + 7 <= n && memcmp(s + j, "del ", 4) == 0) j += 4;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: else if (j + 6 <= n && memcmp(s + j, "de ", 3) == 0) j += 3;
 */
        else if (j + 6 <= n && memcmp(s + j, "de ", 3) == 0) j += 3;
        else {
            i = j;
            continue;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (j + 4 <= n && ozmvm_first_year_20xx(s + j, 4) > 0) return v;
 */
        if (j + 4 <= n && ozmvm_first_year_20xx(s + j, 4) > 0) return v;
        i = j;
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}

/*
 * Función oz_trim_span: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:615 · bloque 7
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
static void oz_trim_span(const char* s, size_t* b, size_t* e) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (*b < *e && oz_is_space(s[*b])) ++(*b);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (*e > *b && oz_is_space(s[*e - 1])) --(*e);
}

/*
 * Función oz_parse_i32_span: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:640 · bloque 8
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
static int oz_parse_i32_span(const char* s, size_t b, size_t e, int* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return 0;
 */
    if (!out) return 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:647 · bloque 9
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    oz_trim_span(s, &b, &e);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (b >= e) return 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:653 · bloque 10
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    if (b >= e) return 0;
    int neg = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[b] == '-') {
 */
    if (s[b] == '-') {
        neg = 1;
        ++b;
    }
    int v = 0;
    int seen = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = b; p < e; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_is_digit(s[p])) return 0;
 */
        if (!oz_is_digit(s[p])) return 0;
        seen = 1;
        const int d = s[p] - '0';
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v > (INT32_MAX - d) / 10) return 0;
 */
        if (v > (INT32_MAX - d) / 10) return 0;
        v = v * 10 + d;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!seen) return 0;
 */
    if (!seen) return 0;
    *out = neg ? -v : v;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

/*
 * Función oz_parse_i64_span: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:713 · bloque 11
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
static int oz_parse_i64_span(const char* s, size_t b, size_t e, int64_t* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return 0;
 */
    if (!out) return 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:725 · bloque 12
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    oz_trim_span(s, &b, &e);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (b >= e) return 0;
 */
    if (b >= e) return 0;
    int neg = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (s[b] == '-') {
 */
    if (s[b] == '-') {
        neg = 1;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:740 · bloque 13
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        ++b;
    }
    int64_t v = 0;
    int seen = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = b; p < e; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_is_digit(s[p])) return 0;
 */
        if (!oz_is_digit(s[p])) return 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        seen = 1;
        const int d = s[p] - '0';
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v > (INT64_MAX - d) / 10) return 0;
 */
        if (v > (INT64_MAX - d) / 10) return 0;
        v = v * 10 + d;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!seen) return 0;
 */
    if (!seen) return 0;
    *out = neg ? -v : v;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

/*
 * Función oz_set_span_trimmed: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:796 · bloque 14
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
static void oz_set_span_trimmed(const char* s, size_t b, size_t e, size_t* out_b, size_t* out_e) {
    oz_trim_span(s, &b, &e);
    *out_b = b;
    *out_e = e;
}

/*
 * Función ozmvm_tsv_epi_parse10: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:812 · bloque 15
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
int ozmvm_tsv_epi_parse10(const char* s, size_t n, OzmvmTsvEpiFields* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !out) return 0;
 */
    if (!s || !out) return 0;
    memset(out, 0, sizeof(*out));
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) --n;

    size_t b[10] = {0};
    size_t e[10] = {0};
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    size_t field = 0;
    size_t start = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t p = 0; p <= n; ++p) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p == n || s[p] == '\t') {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:847 · bloque 16
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (p == n || s[p] == '\t') {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (field >= 10) return 0;
 */
            if (field >= 10) return 0;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
            b[field] = start;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
            e[field] = p;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
            ++field;
            start = p + 1;
        }
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (field != 10) return 0;
 */
    if (field != 10) return 0;

/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:882 · bloque 17
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    oz_set_span_trimmed(s, b[0], e[0], &out->entity_begin, &out->entity_end);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_parse_i32_span(s, b[1], e[1], &out->year)) return 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:888 · bloque 18
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    if (!oz_parse_i32_span(s, b[1], e[1], &out->year)) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_parse_i32_span(s, b[2], e[2], &out->epi_week)) return 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:894 · bloque 19
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    if (!oz_parse_i32_span(s, b[2], e[2], &out->epi_week)) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_parse_i32_span(s, b[3], e[3], &out->page)) out->page = 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:900 · bloque 20
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    if (!oz_parse_i32_span(s, b[3], e[3], &out->page)) out->page = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:906 · bloque 21
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    oz_set_span_trimmed(s, b[4], e[4], &out->disease_begin, &out->disease_end);
    oz_set_span_trimmed(s, b[5], e[5], &out->cie10_begin, &out->cie10_end);
    oz_set_span_trimmed(s, b[6], e[6], &out->jurisdiction_begin, &out->jurisdiction_end);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:909 · bloque 22
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    oz_set_span_trimmed(s, b[7], e[7], &out->period_begin, &out->period_end);
    oz_set_span_trimmed(s, b[8], e[8], &out->sex_begin, &out->sex_end);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_parse_i64_span(s, b[9], e[9], &out->value)) out->value = 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:916 · bloque 23
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    if (!oz_parse_i64_span(s, b[9], e[9], &out->value)) out->value = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    return out->year > 0 && out->epi_week >= 1 && out->epi_week <= 53 &&
           out->disease_begin < out->disease_end &&
           out->jurisdiction_begin < out->jurisdiction_end;
}

/*
 * Función oz_nan: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static double oz_nan(void) {
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
    volatile double z = 0.0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:949 · bloque 24
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    return z / z;
}

/*
 * Función oz_clamp01: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static double oz_clamp01(double v) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v < 0.0) return 0.0;
 */
    if (v < 0.0) return 0.0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v > 1.0) return 1.0;
 */
    if (v > 1.0) return 1.0;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return v;
}

/*
 * Función oz_double_cmp: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_double_cmp(const void* a, const void* b) {
    const double da = *(const double*)a;
    const double db = *(const double*)b;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return (da > db) - (da < db);
}

/*
 * Función oz_read_i64_field: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
static int64_t oz_read_i64_field(const void* base, size_t i, size_t stride, size_t offset) {
    int64_t v = 0;
    memcpy(&v, (const unsigned char*)base + i * stride + offset, sizeof(v));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return v;
}

/*
 * Función oz_read_double_field: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
static double oz_read_double_field(const void* base, size_t i, size_t stride, size_t offset) {
    double v = 0.0;
    memcpy(&v, (const unsigned char*)base + i * stride + offset, sizeof(v));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return v;
}

/*
 * Función oz_median_sorted: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static double oz_median_sorted(const double* values, size_t n) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!values || n == 0) return oz_nan();
 */
    if (!values || n == 0) return oz_nan();
    const size_t mid = n / 2;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return (n & 1u) ? values[mid] : (values[mid - 1] + values[mid]) * 0.5;
}

int ozmvm_i64_stats_strided(const void* base, size_t n, size_t stride,
/*
 * Función ozmvm_i64_stats_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1078 · bloque 25
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
                            size_t value_offset, OzmvmI64Stats* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return 0;
 */
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n == 0 || stride == 0) return 0;
 */
    if (!base || n == 0 || stride == 0) return 0;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    int64_t min_v = oz_read_i64_field(base, 0, stride, value_offset);
    int64_t max_v = min_v;
    int64_t sum = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const int64_t v = oz_read_i64_field(base, i, stride, value_offset);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v < min_v) min_v = v;
 */
        if (v < min_v) min_v = v;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (v > max_v) max_v = v;
 */
        if (v > max_v) max_v = v;
        sum += v;
    }
    const double mean = (double)sum / (double)n;
    out->n = n;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    out->sum = sum;
    out->min_value = min_v;
    out->max_value = max_v;
    out->mean = mean;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (n >= 2) {
 */
    if (n >= 2) {
        double ss = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            const double d = (double)oz_read_i64_field(base, i, stride, value_offset) - mean;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1156 · bloque 26
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            ss += d * d;
        }
        out->sd = sqrt(ss / (double)(n - 1));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fabs(mean) > 1.0e-9) out->cv = out->sd / fabs(mean);
 */
        if (fabs(mean) > 1.0e-9) out->cv = out->sd / fabs(mean);
    }
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

int ozmvm_indexed_i64_ols_strided(const void* base, size_t n, size_t stride,
/*
 * Función ozmvm_indexed_i64_ols_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                                  size_t value_offset, double* out_slope, double* out_intercept) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_slope) *out_slope = oz_nan();
 */
    if (out_slope) *out_slope = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_intercept) *out_intercept = oz_nan();
 */
    if (out_intercept) *out_intercept = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n < 2 || stride == 0 || !out_slope || !out_intercept) return 0;
 */
    if (!base || n < 2 || stride == 0 || !out_slope || !out_intercept) return 0;
    double sx = 0.0;
    double sy = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
        sx += (double)i;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        sy += (double)oz_read_i64_field(base, i, stride, value_offset);
    }
    const double mx = sx / (double)n;
    const double my = sy / (double)n;
    double num = 0.0;
    double dx2 = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
        const double dx = (double)i - mx;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        num += dx * ((double)oz_read_i64_field(base, i, stride, value_offset) - my);
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        dx2 += dx * dx;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dx2 <= 0.0) return 0;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1244 · bloque 27
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    if (dx2 <= 0.0) return 0;
    *out_slope = num / dx2;
    *out_intercept = my - (*out_slope) * mx;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

double ozmvm_indexed_i64_theil_sen_strided(const void* base, size_t n, size_t stride,
/*
 * Función ozmvm_indexed_i64_theil_sen_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                                           size_t value_offset, size_t cap, double* out_intercept) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_intercept) *out_intercept = oz_nan();
 */
    if (out_intercept) *out_intercept = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n < 2 || stride == 0) return oz_nan();
 */
    if (!base || n < 2 || stride == 0) return oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (cap < 2 || cap > n) cap = n;
 */
    if (cap < 2 || cap > n) cap = n;
    const size_t sample_n = cap;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    double* xs = (double*)malloc(sample_n * sizeof(double));
    double* ys = (double*)malloc(sample_n * sizeof(double));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!xs || !ys) {
 */
    if (!xs || !ys) {
        free(xs);
        free(ys);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return oz_nan();
    }
    const double step = (double)n / (double)sample_n;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < sample_n; ++i) {
        size_t src = n <= sample_n ? i : (size_t)((double)i * step);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (src >= n) src = n - 1;
 */
        if (src >= n) src = n - 1;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        xs[i] = (double)src;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        ys[i] = (double)oz_read_i64_field(base, src, stride, value_offset);
    }
    const size_t max_slopes = (sample_n * (sample_n - 1)) / 2;
    double* slopes = (double*)malloc(max_slopes * sizeof(double));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!slopes) {
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1337 · bloque 28
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    if (!slopes) {
        free(xs);
        free(ys);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return oz_nan();
    }
    size_t slope_n = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < sample_n; ++i) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (size_t j = i + 1; j < sample_n; ++j) {
            const double dx = xs[j] - xs[i];
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (fabs(dx) < 1.0e-12) continue;
 */
            if (fabs(dx) < 1.0e-12) continue;
            slopes[slope_n++] = (ys[j] - ys[i]) / dx;
        }
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (slope_n == 0) {
 */
    if (slope_n == 0) {
        free(slopes);
        free(xs);
        free(ys);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return oz_nan();
    }
    qsort(slopes, slope_n, sizeof(double), oz_double_cmp);
    const double slope = oz_median_sorted(slopes, slope_n);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_intercept && isfinite(slope)) {
 */
    if (out_intercept && isfinite(slope)) {
        double* intercepts = (double*)malloc(sample_n * sizeof(double));
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (intercepts) {
 */
        if (intercepts) {
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
            for (size_t i = 0; i < sample_n; ++i) intercepts[i] = ys[i] - slope * xs[i];
            qsort(intercepts, sample_n, sizeof(double), oz_double_cmp);
            *out_intercept = oz_median_sorted(intercepts, sample_n);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1409 · bloque 29
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            free(intercepts);
        }
    }
    free(slopes);
    free(xs);
    free(ys);
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return slope;
}

double ozmvm_xy_pearson_strided(const void* base, size_t n, size_t stride,
/*
 * Función ozmvm_xy_pearson_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                                size_t x_offset, size_t y_offset) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n < 2 || stride == 0) return oz_nan();
 */
    if (!base || n < 2 || stride == 0) return oz_nan();
    double sx = 0.0;
    double sy = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        sx += oz_read_double_field(base, i, stride, x_offset);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        sy += oz_read_double_field(base, i, stride, y_offset);
    }
    const double mx = sx / (double)n;
    const double my = sy / (double)n;
    double num = 0.0;
    double dx2 = 0.0;
    double dy2 = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double dx = oz_read_double_field(base, i, stride, x_offset) - mx;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double dy = oz_read_double_field(base, i, stride, y_offset) - my;
        num += dx * dy;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1481 · bloque 30
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        dx2 += dx * dx;
        dy2 += dy * dy;
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (dx2 <= 0.0 || dy2 <= 0.0) return oz_nan();
 */
    if (dx2 <= 0.0 || dy2 <= 0.0) return oz_nan();
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return num / sqrt(dx2 * dy2);
}

int ozmvm_xy_ols_strided(const void* base, size_t n, size_t stride,
                         size_t x_offset, size_t y_offset, const double* weights,
/*
 * Función ozmvm_xy_ols_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                         double* out_slope, double* out_intercept) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_slope) *out_slope = oz_nan();
 */
    if (out_slope) *out_slope = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (out_intercept) *out_intercept = oz_nan();
 */
    if (out_intercept) *out_intercept = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n < 2 || stride == 0 || !out_slope || !out_intercept) return 0;
 */
    if (!base || n < 2 || stride == 0 || !out_slope || !out_intercept) return 0;
    double sw = 0.0;
    double sx = 0.0;
    double sy = 0.0;
    double sxx = 0.0;
    double sxy = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
        double w = weights ? weights[i] : 1.0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!(w > 0.0) || !isfinite(w)) continue;
 */
        if (!(w > 0.0) || !isfinite(w)) continue;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double x = oz_read_double_field(base, i, stride, x_offset);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double y = oz_read_double_field(base, i, stride, y_offset);
        sw += w;
        sx += w * x;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1556 · bloque 31
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        sy += w * y;
        sxx += w * x * x;
        sxy += w * x * y;
    }
    const double denom = sw * sxx - sx * sx;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (sw <= 0.0 || fabs(denom) < 1.0e-12) return 0;
 */
    if (sw <= 0.0 || fabs(denom) < 1.0e-12) return 0;
    *out_slope = (sw * sxy - sx * sy) / denom;
    *out_intercept = (sy - (*out_slope) * sx) / sw;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

int ozmvm_xy_fit_metrics_strided(const void* base, size_t n, size_t stride,
                                 size_t x_offset, size_t y_offset,
/*
 * Función ozmvm_xy_fit_metrics_strided: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
                                 double slope, double intercept, OzmvmLinearFitD* out) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!out) return 0;
 */
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->slope = slope;
    out->intercept = intercept;
    out->index = oz_nan();
    out->rmse = oz_nan();
    out->mae = oz_nan();
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    out->pearson = oz_nan();
    out->r2 = oz_nan();
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!base || n < 2 || stride == 0 || !isfinite(slope) || !isfinite(intercept)) return 0;
 */
    if (!base || n < 2 || stride == 0 || !isfinite(slope) || !isfinite(intercept)) return 0;
    double sx = 0.0;
    double sy = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1623 · bloque 32
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        sx += oz_read_double_field(base, i, stride, x_offset);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        sy += oz_read_double_field(base, i, stride, y_offset);
    }
    const double mx = sx / (double)n;
    const double my = sy / (double)n;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    double se = 0.0;
    double ae = 0.0;
    double rss = 0.0;
    double tss = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double x = oz_read_double_field(base, i, stride, x_offset);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
        const double y = oz_read_double_field(base, i, stride, y_offset);
        const double pred = intercept + slope * x;
        const double err = y - pred;
        const double dy = y - my;
        se += err * err;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        ae += fabs(err);
        rss += err * err;
        tss += dy * dy;
    }
    out->mean_x = mx;
    out->mean_y = my;
    out->rmse = sqrt(se / (double)n);
    out->mae = ae / (double)n;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1677 · bloque 33
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    out->pearson = ozmvm_xy_pearson_strided(base, n, stride, x_offset, y_offset);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (tss > 1.0e-12) {
 */
    if (tss > 1.0e-12) {
        const double pseudo_r2 = 1.0 - rss / tss;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        out->r2 = pseudo_r2;
        out->index = (slope < 0.0 ? -1.0 : 1.0) * oz_clamp01(fabs(pseudo_r2));
    } else {
        double sp = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            const double x = oz_read_double_field(base, i, stride, x_offset);
            sp += intercept + slope * x;
        }
        const double mp = sp / (double)n;
        double num = 0.0;
        double pp2 = 0.0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
        double yy2 = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
        for (size_t i = 0; i < n; ++i) {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            const double x = oz_read_double_field(base, i, stride, x_offset);
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
            const double y = oz_read_double_field(base, i, stride, y_offset);
            const double dp = intercept + slope * x - mp;
            const double dy = y - my;
            num += dp * dy;
            pp2 += dp * dp;
            yy2 += dy * dy;
        }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (pp2 > 0.0 && yy2 > 0.0) out->index = num / sqrt(pp2 * yy2);
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1746 · bloque 34
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        if (pp2 > 0.0 && yy2 > 0.0) out->index = num / sqrt(pp2 * yy2);
    }
    out->valid = 1;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

/*
 * Función oz_mean_range: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static double oz_mean_range(const double* values, size_t first, size_t last) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!values || first >= last) return oz_nan();
 */
    if (!values || first >= last) return oz_nan();
    double s = 0.0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = first; i < last; ++i) s += values[i];
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return s / (double)(last - first);
}

/*
 * Función ozmvm_aggregate_double_values: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
double ozmvm_aggregate_double_values(double* values, size_t n, int mode) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!values || n == 0) return oz_nan();
 */
    if (!values || n == 0) return oz_nan();
    size_t m = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    for (size_t i = 0; i < n; ++i) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (isfinite(values[i])) values[m++] = values[i];
 */
        if (isfinite(values[i])) values[m++] = values[i];
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (m == 0) return oz_nan();
 */
    if (m == 0) return oz_nan();
    qsort(values, m, sizeof(double), oz_double_cmp);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (mode < 0) mode = 0;
 */
    if (mode < 0) mode = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (mode > 5) mode = 5;
 */
    if (mode > 5) mode = 5;
    switch (mode) {
        case 1:
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return oz_median_sorted(values, m);
        case 2: {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (m <= 4) return oz_mean_range(values, 0, m);
 */
            if (m <= 4) return oz_mean_range(values, 0, m);
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:1854 · bloque 35
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            const size_t trim = m / 5 > 1 ? m / 5 : 1;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · core.c:1860 · bloque 36
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
            return oz_mean_range(values, trim, m - trim);
        }
        case 3:
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return values[m - 1];
        case 4: {
            const size_t idx = (size_t)floor(0.90 * (double)(m - 1));
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return values[idx < m ? idx : m - 1];
        }
        case 5: {
            int best_bin = (int)lround(values[0]);
            int best_n = 0;
            int cur_bin = best_bin;
            int cur_n = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
            for (size_t i = 0; i < m; ++i) {
                const int bin = (int)lround(values[i]);
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (bin == cur_bin) {
 */
                if (bin == cur_bin) {
                    ++cur_n;
                } else {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (cur_n > best_n) {
 */
                    if (cur_n > best_n) {
                        best_n = cur_n;
                        best_bin = cur_bin;
                    }
                    cur_bin = bin;
                    cur_n = 1;
                }
            }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (cur_n > best_n) best_bin = cur_bin;
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:1916 · bloque 37
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
            if (cur_n > best_n) best_bin = cur_bin;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return (double)best_bin;
        }
        default:
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return oz_mean_range(values, 0, m);
    }
}

/*
 * Función oz_cstr_len_cap: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static size_t oz_cstr_len_cap(const char* s, size_t n) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s) return 0;
 */
    if (!s) return 0;
    size_t len = 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (len < n && s[len] != '\0') ++len;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return len;
}

/*
 * Función oz_suffix_token_line: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_suffix_token_line(const char* s, size_t begin, size_t end) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || begin >= end) return 0;
 */
    if (!s || begin >= end) return 0;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (begin < end && s[begin] == ' ') ++begin;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (begin >= end) return 0;
 */
    if (begin >= end) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (end - begin == 1) {
 */
    if (end - begin == 1) {
        const char c = s[begin];
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
        return (c >= '1' && c <= '9') || c == 'a' || c == 'b';
    }
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (end - begin == 2 && s[begin] == '1' && s[begin + 1] == '2') return 1;
 */
    if (end - begin == 2 && s[begin] == '1' && s[begin + 1] == '2') return 1;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 0;
}

/*
 * Función oz_remove_suffix_literal: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_remove_suffix_literal(char* s, size_t* len, const char* suffix) {
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz.
 */
    const size_t slen = suffix ? strlen(suffix) : 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !len || slen == 0 || *len < slen) return 0;
 */
    if (!s || !len || slen == 0 || *len < slen) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (memcmp(s + *len - slen, suffix, slen) != 0) return 0;
 */
    if (memcmp(s + *len - slen, suffix, slen) != 0) return 0;
    *len -= slen;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:2058 · bloque 38
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    while (*len > 0 && s[*len - 1] == ' ') --(*len);
    s[*len] = '\0';
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

/*
 * Función oz_remove_suffix_line: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
static int oz_remove_suffix_line(char* s, size_t* len, const char* marker) {
    const size_t mlen = marker ? strlen(marker) : 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || !len || mlen == 0 || *len <= mlen) return 0;
 */
    if (!s || !len || mlen == 0 || *len <= mlen) return 0;
    size_t p = *len;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (p > 0 && s[p - 1] != ' ') --p;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!oz_suffix_token_line(s, p, *len)) return 0;
 */
    if (!oz_suffix_token_line(s, p, *len)) return 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (p < mlen || memcmp(s + p - mlen, marker, mlen) != 0) return 0;
 */
    if (p < mlen || memcmp(s + p - mlen, marker, mlen) != 0) return 0;
    *len = p - mlen;
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (*len > 0 && s[*len - 1] == ' ') --(*len);
    s[*len] = '\0';
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return 1;
}

/*
 * Función ozmvm_mobility_station_icon_key_inplace: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
size_t ozmvm_mobility_station_icon_key_inplace(char* s, size_t n) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!s || n == 0) return 0;
 */
    if (!s || n == 0) return 0;
    size_t len = oz_cstr_len_cap(s, n);
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (len > 0 && s[len - 1] == ' ') s[--len] = '\0';
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (len >= 6 && memcmp(s, "metro ", 6) == 0) {
 */
    if (len >= 6 && memcmp(s, "metro ", 6) == 0) {
        memmove(s, s + 6, len - 5);
        len -= 6;
    }
/*
 * Recorrido controlado: la iteración siguiente debe evaluarse por complejidad, localidad de memoria y posibilidad de datos parciales, no solo por corrección funcional en entradas pequeñas.
 * En rutas interactivas conviene preservar límites, salidas tempranas y orden determinista para no introducir pausas visibles ni resultados dependientes del azar de contenedores o hilos.
 * Este patrón se integra dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz y por eso cualquier incremento de trabajo por elemento debe ser deliberado.
 */
    while (len > 0 && s[len - 1] == ' ') s[--len] = '\0';
    oz_remove_suffix_literal(s, &len, " metro");
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · core.c:2161 · bloque 39
 * Este tramo pertenece a núcleo C de bajo nivel, donde conviene mantener memoria, límites y rutas de error explícitas; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    oz_remove_suffix_literal(s, &len, " estacion");
    oz_remove_suffix_line(s, &len, " correspondencia linea ");
    oz_remove_suffix_line(s, &len, " linea ");
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
    return len;
}

/*
 * Función ozmvm_lag_to_days: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
int ozmvm_lag_to_days(int value, int unit) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (value < 0) value = 0;
 */
    if (value < 0) value = 0;
    switch (unit) {
        case 1: return value > 260 ? 1820 : value * 7;
        case 2: return value > 60 ? 1800 : value * 30;
        case 3: return value > 5 ? 1825 : value * 365;
        default: return value > 1825 ? 1825 : value;
    }
}

/*
 * Función ozmvm_lag_value_from_days: construye, convierte o alinea una representación temporal y debe conservar reglas calendario explícitas para que las series históricas no se desplacen silenciosamente entre fuentes.
 * Contexto: pertenece a núcleo C de bajo nivel: rutinas mínimas, portables y estables que no dependen de C++ ni de ImGui. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
/*
 * Frontera funcional: la rutina que sigue concentra una transformación identificable y debe leerse como unidad de responsabilidad, aun cuando internamente combine validación, normalización y escritura de estado.
 * La firma delimita el contrato observable; cualquier caché, tolerancia a errores o efecto lateral debe permanecer justificable desde los argumentos y el estado documentado cercano.
 * Para el núcleo C de bajo nivel, destinado a operaciones estrictas, simples y portables que no deben depender de la capa C++ de interfaz, esta separación evita que un ajuste de interfaz termine alterando silenciosamente la semántica analítica.
 */
int ozmvm_lag_value_from_days(int days, int unit) {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (days < 0) days = 0;
 */
    if (days < 0) days = 0;
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (days > 1825) days = 1825;
 */
    if (days > 1825) days = 1825;
    switch (unit) {
        case 1: return (days + 3) / 7;
        case 2: return (days + 15) / 30;
        case 3: return (days + 182) / 365;
        default: return days;
    }
}
