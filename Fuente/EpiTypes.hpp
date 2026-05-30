

/*
 * Archivo: EpiTypes.hpp
 * Papel técnico: modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * Comentario de mantenimiento: este bloque fue reconstruido desde cero para documentar el comportamiento vigente sin arrastrar notas obsoletas de iteraciones previas. La intención es que cada lectura futura distinga con claridad entre contratos de datos, puntos de extensión, supuestos de rendimiento, rutas de E/S y límites que no deben fusionarse por conveniencia.
 * Regla de no regresión: los comentarios describen invariantes y responsabilidades, pero no introducen lógica; cualquier cambio funcional debe ser deliberado, probado y separado de la documentación.
 */
#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · EpiTypes.hpp:23 · bloque 1
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
#include <mutex>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/*
 * Espacio de nombres interno: encapsula auxiliares con enlace local para reducir superficie pública, evitar colisiones entre módulos y dejar claro que la función de estas rutinas es apoyar al archivo, no volverse API estable.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · EpiTypes.hpp:38 · bloque 2
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace epi {

/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · fusión mensual sin regresión · EpiTypes.hpp:45 · bloque 3
 * La operación cercana participa en la fusión de datos normalizados hacia el mes correspondiente; el criterio rector es que el mes es el contenedor lógico, no el dominio temático ni el formato de origen.
 * Cuando un importador reconstruye un núcleo, sólo debe sustituir el subconjunto de registros que semánticamente controla; los demás núcleos IXIPTLAH del mismo archivo mensual son propiedad de otros flujos y deben conservarse byte a byte siempre que no estén explícitamente dentro del alcance de purga.
 * La estrategia correcta es preparar staging, validar cardinalidad, purgar selectivamente los registros antiguos del núcleo reconstruido, anexar los registros nuevos y publicar de forma atómica; hacerlo por archivos separados por categoría simplificaría un caso local pero degradaría la arquitectura global.
 * Este diseño también reduce daño por corrupción: un registro malformado no debe convertir en irrecuperable el resto del archivo, y una categoría fallida no debe bloquear la consulta de categorías ya sanas del mismo mes.
 * Cualquier cambio futuro debe demostrar que conserva la unidad mensual, la independencia de núcleos, la trazabilidad de fuente y la posibilidad de lectura selectiva sin cargar dominios innecesarios.
 */
namespace fs = std::filesystem;


/*
 * Tipo Rect: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct Rect {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:58 · bloque 4
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    double x0 = 0.0;
    double y0 = 0.0;
    double x1 = 0.0;
    double y1 = 0.0;

/*
 * Función width: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    [[nodiscard]] double width() const noexcept { return x1 - x0; }
/*
 * Función height: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    [[nodiscard]] double height() const noexcept { return y1 - y0; }
/*
 * Función cx: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    [[nodiscard]] double cx() const noexcept { return (x0 + x1) * 0.5; }
/*
 * Función cy: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    [[nodiscard]] double cy() const noexcept { return (y0 + y1) * 0.5; }
/*
 * Función valid: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    [[nodiscard]] bool valid() const noexcept { return x1 >= x0 && y1 >= y0; }

/*
 * Función include: implementa una etapa específica del módulo y mantiene localizados los supuestos necesarios para no dispersar reglas de negocio en llamadores.
 * Contexto: pertenece a modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización. Las entradas deben interpretarse según contratos ya normalizados por el módulo llamador, y cualquier tolerancia a datos sucios debe permanecer explícita para que un refactor no confunda ausencia de datos con cero, falso o selección vacía.
 * Invariante de mantenimiento: no trasladar efectos laterales ocultos a esta rutina ni convertirla en punto de acceso global; si necesita caché, bloqueo, cancelación o reporte de progreso, esos mecanismos deben quedar visibles en la firma o en el estado compartido documentado del módulo.
 */
    void include(const Rect& r) noexcept {
/*
 * Barrera semántica: esta condición no solo decide flujo, también protege una suposición de dominio que puede fallar por datos incompletos, recursos no disponibles o interacción del usuario en tiempo real.
 * La rama debe conservar diferencia entre ausencia, valor nulo, valor cero y error recuperable; fusionar esos casos suele producir regresiones difíciles de observar en mapas o series temporales.
 * La línea evaluada es: if (!valid() || (x0 == 0.0 && x1 == 0.0 && y0 == 0.0 && y1 == 0.0)) {
 */
        if (!valid() || (x0 == 0.0 && x1 == 0.0 && y0 == 0.0 && y1 == 0.0)) {
            *this = r;
/*
 * Salida contractual: este retorno fija el valor observable por el llamador y debe distinguir resultado válido, ausencia de resultado, degradación recuperable y error real.
 * No conviene convertir esta salida en atajo para ocultar estados intermedios inconsistentes; si la función publica éxito, los invariantes ya deben estar restaurados.
 * Dentro de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, esta disciplina evita que una capa posterior compense silenciosamente una falla anterior.
 */
            return;
        }
/*
 * Declaración min: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
        x0 = std::min(x0, r.x0);
/*
 * Declaración min: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
        y0 = std::min(y0, r.y0);
/*
 * Declaración max: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:133 · bloque 5
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
        x1 = std::max(x1, r.x1);
/*
 * Declaración max: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
        y1 = std::max(y1, r.y1);
    }
};


/*
 * Tipo Token: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct Token {
    int page = 0;
    std::string text;
    std::string norm;
    Rect box;
};


/*
 * Tipo PageText: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct PageText {
    int page = 0;
    double width = 0.0;
    double height = 0.0;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<Token> tokens;
};


/*
 * Tipo PdfDocument: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:191 · bloque 6
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
struct PdfDocument {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:197 · bloque 7
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    fs::path pdf_path;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:203 · bloque 8
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string file_name;
    std::string stable_id;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int bulletin_year = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int bulletin_week = 0;
    int source_page_count = 0;
    int first_extracted_page = 1;
    int last_extracted_page = 0;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<PageText> pages;
};


/*
 * Tipo Jurisdiction: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct Jurisdiction {
    std::string id;
    std::string canonical;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> aliases;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> aliases_norm;
};


/*
 * Tipo Disease: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct Disease {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:266 · bloque 9
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string id;
    std::string canonical;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    std::string group;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> cie10;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> cie10_norm;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> aliases;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<std::string> aliases_norm;
};


/*
 * Tipo RowBand: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct RowBand {
    std::string jurisdiction_id;
    std::string jurisdiction;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    Rect label_box;
    double y_mid = 0.0;
    double y0 = 0.0;
    double y1 = 0.0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:322 · bloque 10
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    int line_index = -1;
};


/*
 * Tipo ColumnBand: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct ColumnBand {
    int index = -1;
    double x_mid = 0.0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    double x0 = 0.0;
    double x1 = 0.0;
    std::string period = "unknown";
    std::string sex = "total";
    std::string disease_id = "unknown";
    std::string disease = "unknown";
    std::string cie10 = "";
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    std::string source_year = "";
    std::string header_text = "";
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    std::string role = "";
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:363 · bloque 11
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string expected_role = "";
    std::string group_layout_note = "";
    int group_index = -1;
    Rect header_box;
    Rect disease_box;
    Rect cie10_box;
    double header_confidence = 0.0;
};


/*
 * Tipo ParsedValue: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · EpiTypes.hpp:382 · bloque 12
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
struct ParsedValue {
    std::string raw;
    std::optional<int64_t> value;
    Rect box;
};


/*
 * Tipo Observation: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct Observation {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:404 · bloque 13
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::string pdf_file;
    std::string pdf_id;
    int page = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int bulletin_year = 0;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    int bulletin_week = 0;
    std::string table_id;
    std::string table_title;
    std::string disease_id;
    std::string disease;
    std::string cie10;
    std::string jurisdiction_id;
    std::string jurisdiction;
/*
 * Semántica temporal: el cálculo cercano manipula fechas civiles, semanas, meses, horas o minutos, por lo que debe evitar aproximaciones que confundan calendario gregoriano, escala visual y periodo de agregación.
 * Las conversiones deben mantener bordes inclusivos y exclusivos explícitos; un desplazamiento de una unidad puede duplicar o invisibilizar datos en la línea histórica.
 * La navegación futura acotada y las etiquetas editables dependen de conservar esta separación entre instante seleccionado, intervalo mostrado y granularidad de edición.
 */
    std::string source_year;
    std::string period;
    std::string sex;
    std::string raw_value;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:435 · bloque 14
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    int64_t value = 0;
    double confidence = 0.0;
    Rect cell_box;
    std::string validation_rule;
};


/*
 * Tipo QuarantineItem: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct QuarantineItem {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:457 · bloque 15
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::string pdf_file;
    std::string pdf_id;
    int page = 0;
    std::string table_id;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    std::string column_key;
    std::string reason;
    std::string detail;
};


/*
 * Tipo TableCandidate: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct TableCandidate {
    std::string table_id;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:483 · bloque 16
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    std::string table_title;
    int page = 0;
    Rect page_box;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    std::vector<RowBand> rows;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    std::vector<ColumnBand> columns;
/*
 * Semántica espacial: el fragmento siguiente opera con coordenadas, teselas, escalas o proyecciones; aquí no son equivalentes longitud/latitud, metros, píxeles y coordenadas normalizadas.
 * La precisión visual depende de mantener separados centro cartográfico, zoom interactivo, nivel de detalle de tesela y límites geográficos autoritativos.
 * Cualquier simplificación geométrica debe validarse contra navegación extrema para no producir saltos, cierres incompletos de perímetro o desalineación de capas.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · EpiTypes.hpp:503 · bloque 17
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    std::map<std::pair<std::string, int>, ParsedValue> cells;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::set<std::pair<std::string, int>> duplicate_cells;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<Observation> accepted;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<QuarantineItem> quarantine;
};


/*
 * Tipo PipelineStats: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct PipelineStats {
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:535 · bloque 18
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    int pdf_total = 0;
    int pdf_done = 0;
    int pages_total = 0;
    int pages_done = 0;
    int detail_total = 0;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    int detail_done = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · EpiTypes.hpp:546 · bloque 19
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    double progress = 0.0;
    int pages_with_tables = 0;
    int tables_detected = 0;
    int64_t observations_accepted = 0;
    int64_t quarantine_items = 0;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:551 · bloque 20
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::string current_pdf;
    int current_page = 0;
    std::string status = "listo";
};


/*
 * Tipo ExtractionPreview: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct ExtractionPreview {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:572 · bloque 21
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::string pdf_file;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path page_image;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:579 · bloque 22
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    int pdf_index = 0;
    int pdf_total = 0;
    int page = 0;
    double page_width = 0.0;
    double page_height = 0.0;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<Token> tokens;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    std::vector<RowBand> rows;
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
    std::vector<ColumnBand> columns;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<Observation> accepted;
/*
 * Manejo de contenedor: la estructura cercana puede alterar orden, cardinalidad, unicidad o propiedad de memoria, por lo que su semántica importa tanto como el tipo elegido.
 * Vectores favorecen localidad y recorrido estable; mapas y conjuntos codifican unicidad o búsqueda; cambiar entre ellos sin revisar invariantes modifica rendimiento y resultados visibles.
 * La reserva, limpieza y reutilización deben analizarse frente a sesiones largas para evitar crecimiento residente innecesario.
 */
    std::vector<QuarantineItem> quarantine;
    std::string status;
};


/*
 * Tipo AppOptions: agrupa estado o registros usados por modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La estructura debe conservar una semántica de propiedad explícita: los campos representan datos ya normalizados o buffers temporales con vida útil definida por el llamador. Evitar mezclar aquí decisiones de presentación con reglas de lectura, porque esa contaminación dificulta la serialización, las pruebas y la carga diferida.
 */
/*
 * Definición estructural: el bloque siguiente fija una forma de datos que funciona como contrato interno, no como simple agrupación sintáctica de campos.
 * La estabilidad de esta forma es crítica porque otros módulos pueden depender de nombres, rangos y semántica de ausencia; cambiarla requiere migración explícita, no sustituciones locales.
 * En el contexto de los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, este tipo ayuda a separar representación de memoria, lectura externa y decisiones visuales posteriores.
 */
struct AppOptions {
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path input_dir;


/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:641 · bloque 23
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    std::vector<fs::path> explicit_pdfs;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path output_dir;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · invariante técnico distribuido · EpiTypes.hpp:653 · bloque 24
 * Este tramo pertenece a tipos epidemiológicos base, que deben preservar identidad, jurisdicción, variable, fecha y fuente; su lectura correcta exige distinguir dato crudo, dato normalizado, estado gráfico, caché transitoria y persistencia oficial.
 * La regla de mantenimiento es conservar fronteras semánticas: leer no es aceptar, aceptar no es publicar, publicar no es renderizar, y renderizar no debe retroalimentar la verdad persistida.
 * Cuando el flujo toque importación, el resultado estable debe terminar en IXIPTLAH mensual centralizado; cualquier archivo auxiliar debe considerarse caché, bitácora o staging, nunca fuente canónica paralela.
 * Los comentarios aquí no sustituyen validación en tiempo de ejecución: documentan por qué la estructura cercana no debe simplificarse eliminando controles de tamaño, esquema, fecha, tipo, índice o atomicidad.
 * Al refactorizar, preservar la capacidad de degradar localmente, reportar causa, continuar con datos sanos y no contaminar otros núcleos internos del mes.
 */
    fs::path config_dir;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path log_dir;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path runtime_dir;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:671 · bloque 25
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    fs::path pdftotext;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:677 · bloque 26
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    fs::path pdftoppm;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
    fs::path tesseract;
/*
 * Frontera de E/S: el acceso siguiente cruza desde memoria controlada hacia sistema de archivos, red, proceso externo o recurso empacado; por eso debe tolerar rutas ausentes y fallas parciales.
 * La lectura no debe asumir atomicidad perfecta ni codificación homogénea; las fuentes oficiales y cachés locales pueden estar truncadas, duplicadas o temporalmente bloqueadas.
 * En los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, reportar degradación sin corromper el estado persistente es más importante que fallar tarde con datos aparentemente válidos.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · EpiTypes.hpp:689 · bloque 27
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
    fs::path geojson;
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
    bool dashboard = true;
    bool render_pages = true;
    bool stop_on_error = false;
    bool resume = true;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:699 · bloque 28
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    int limit_pdfs = 0;
    int skip_front_pages = 10;
    int skip_back_pages = 1;
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:702 · bloque 29
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
    int max_pages_per_pdf = 0;
    int render_dpi = 72;
/*
 * Declaración void: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Punto de mantenimiento fino: el bloque inmediato participa en una cadena mayor de estado y por ello debe modificarse entendiendo qué datos recibe ya normalizados y qué datos todavía son provisionales.
 * La regla práctica es no desplazar validación, conversión ni efectos laterales a otra capa para “simplificar” este tramo; esa simplificación suele crear acoplamientos invisibles.
 * Contexto técnico local: los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · EpiTypes.hpp:713 · bloque 30
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    std::function<void(const PipelineStats&)> progress_callback;
/*
 * Declaración void: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    std::function<void(const ExtractionPreview&)> preview_callback;
/*
 * Declaración bool: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
    std::function<bool()> pause_requested;
/*
 * Declaración bool: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · concurrencia visible de importación · EpiTypes.hpp:728 · bloque 31
 * Los importadores pueden ejecutarse mientras la interfaz sigue renderizando; por tanto, cada variable de progreso, bandera de cancelación o hilo auxiliar debe tratarse como frontera de sincronización, no como simple detalle cosmético.
 * La UI sólo debe observar estados consistentes: detectados, indexados, aceptados, cuarentena y mensaje humano deben avanzar de forma que un cierre de ventana o una cancelación no deje archivos mensuales parcialmente publicados.
 * Los importadores de sopa de letras requieren granularidad de avance porque procesan fuentes grandes y ruidosas; el usuario necesita saber qué se está leyendo, pero la persistencia IXIPTLAH sólo debe publicarse cuando el núcleo reconstruido sea coherente.
 * Evitar lecturas no bloqueadas de estructuras mutables complejas; cuando baste con atómicos o copias bajo mutex, esa elección debe permanecer local y documentada para impedir carreras difíciles de reproducir.
 * La progresión visual jamás debe ser usada como criterio de verdad de datos: el dato válido es el registro materializado y verificable dentro del IXIPTLAH mensual.
 */
    std::function<bool()> cancel_requested;
};

/*
 * Declaración rect_to_json: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * Normalización de datos: la operación siguiente transforma representación externa en claves internas, registros tipados o estructuras persistibles.
 * Debe preservarse la trazabilidad entre fuente, campo leído y valor materializado; corregir textos o formatos sin conservar intención puede romper deduplicación, filtros o índices temporales.
 * Este punto pertenece a los tipos epidemiológicos fundamentales, donde nombres, códigos, periodos y agregados deben mantener una semántica estable entre importación y visualización, donde robustez ante heterogeneidad vale más que asumir un esquema ideal.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · normalización semántica de sopa de letras · EpiTypes.hpp:740 · bloque 32
 * El parser cercano opera sobre texto o tablas que ya perdieron parte de su estructura original; cada regla debe compensar esa pérdida sin inventar significado epidemiológico, atmosférico o geográfico que no esté sustentado por la fuente.
 * La normalización debe preservar trazabilidad granular: semana detectada, año inferido, jurisdicción, variable, unidad, fuente, página o archivo, porque la fusión posterior necesita diferenciar corrección legítima de deduplicación accidental.
 * Las heurísticas deben ser conservadoras ante ambigüedad: es preferible mandar una fila a cuarentena que contaminar el IXIPTLAH mensual con una observación cuya fecha, dominio o unidad no pueda defenderse técnicamente.
 * No trasladar reglas de interpretación al render ni al panel gráfico; la interfaz sólo debe mostrar datos ya tipados, y la responsabilidad de convertir la sopa de letras en registros corresponde al importador.
 * Cualquier ampliación de fuente debe incluir el camino completo: detección, lectura, normalización, validación, materialización en núcleo mensual, lectura selectiva y visualización, sin atajos por archivos temáticos externos.
 */
[[nodiscard]] std::string rect_to_json(const Rect& r);
/*
 * Declaración rect_to_csv: expone una operación de modelo de datos epidemiológico: jurisdicciones, enfermedades, observaciones, series temporales y estructuras compartidas entre extracción, tablas y visualización.
 * La firma es el contrato que consumen otros módulos; mantener tipos, orden de parámetros y semántica de errores evita que el acoplamiento se desplace hacia conversiones implícitas o estados globales difíciles de depurar.
 */
/*
 * TLALPOWA_COMENTARIO_TECNICO_V3 · importadores de sopa de letras · EpiTypes.hpp:745 · bloque 33
 * Los importadores de boletines deben entenderse aquí como importadores de sopa de letras: la entrada real rara vez es una tabla limpia; suele ser una mezcla de PDF, HTML, CSV, enlaces, encabezados repetidos, notas editoriales, semanas codificadas en nombres de archivo y fragmentos tabulares con columnas desplazadas.
 * La responsabilidad técnica de este tramo no es preservar la apariencia de esa sopa, sino aislar señales verificables, normalizarlas con reglas explícitas, conservar trazabilidad de fuente y convertirlas en registros tipados que puedan sobrevivir a deduplicación, reintentos y reconstrucciones parciales.
 * Toda salida aceptada por estos importadores debe tender al IXIPTLAH mensual que corresponda por fecha del dato; queda documentado como invariante que no deben proliferar IXIPTLAH separados por tipo de información, por origen o por familia de boletín.
 * La centralización mensual permite que epidemiología, atmósfera, movilidad, cartografía histórica y metadatos convivan en un mismo archivo cronológico, pero separados internamente por núcleos de registro, de modo que el tiempo sea el eje de integración y no una consecuencia accidental del nombre de carpeta.
 * Si una fuente llega incompleta, corrupta, duplicada o con codificación defectuosa, el importador debe degradar de forma local y auditable: rechazar sólo lo inválido, registrar la causa y evitar que el resto del mes pierda su coherencia binaria o analítica.
 */
[[nodiscard]] std::string rect_to_csv(const Rect& r);

}
