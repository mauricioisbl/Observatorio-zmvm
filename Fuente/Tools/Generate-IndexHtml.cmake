
# Archivo CMake: Generate-IndexHtml.cmake
# Papel técnico: script CMake auxiliar de Tlalpowa.
# Estos comentarios documentan contratos de configuración, no cambian la gráfica de dependencias; cada instrucción debe conservar su orden porque CMake evalúa variables, targets y FetchContent de forma incremental.

# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW)
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()


# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_WEB_OUTPUT_DIR OR "${TLALPOWA_WEB_OUTPUT_DIR}" STREQUAL "")
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(DEFINED TLALPOWA_INDEX_OUTPUT AND NOT "${TLALPOWA_INDEX_OUTPUT}" STREQUAL "")
    get_filename_component(TLALPOWA_WEB_OUTPUT_DIR "${TLALPOWA_INDEX_OUTPUT}" DIRECTORY)
# Bloque else: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  else()
# Bloque message: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    message(FATAL_ERROR "TLALPOWA_WEB_OUTPUT_DIR no fue definido para Generate-IndexHtml.cmake")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()

# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_WEB_RUNTIME_OUTPUT_DIR)
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(TLALPOWA_WEB_RUNTIME_OUTPUT_DIR "")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()

# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_BUILD_PROJECT_NAME OR "${TLALPOWA_BUILD_PROJECT_NAME}" STREQUAL "")
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(TLALPOWA_BUILD_PROJECT_NAME "Tlalpowa")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()

# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_INDEX_TEMPLATE)
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(TLALPOWA_INDEX_TEMPLATE "")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_CSS_TEMPLATE)
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(TLALPOWA_CSS_TEMPLATE "")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
if(NOT DEFINED TLALPOWA_JS_TEMPLATE)
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(TLALPOWA_JS_TEMPLATE "")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endif()

string(TIMESTAMP TLALPOWA_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")


# Bloque function: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
function(tlalpowa_read_or_fallback out_var template_path fallback_text)
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(template_path AND EXISTS "${template_path}")
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(READ "${template_path}" _text)
# Bloque else: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  else()
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    set(_text "${fallback_text}")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
  string(REPLACE "@TLALPOWA_BUILD_PROJECT_NAME@" "${TLALPOWA_BUILD_PROJECT_NAME}" _text "${_text}")
  string(REPLACE "@TLALPOWA_BUILD_TIMESTAMP@" "${TLALPOWA_BUILD_TIMESTAMP}" _text "${_text}")
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(${out_var} "${_text}" PARENT_SCOPE)
endfunction()

# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
set(TLALPOWA_INDEX_FALLBACK "<!doctype html>\n<html lang=\"es\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>@TLALPOWA_BUILD_PROJECT_NAME@</title><link rel=\"stylesheet\" href=\"main.css\"></head><body><main class=\"app\"><header class=\"toolbar\"><button class=\"tab active\">Principal</button><button class=\"tab\">Gráfica 1</button><button class=\"command primary\">Importar Datos</button></header><section class=\"workspace\"><div class=\"map\"><div class=\"label\">Zona Metropolitana del Valle de México</div></div><aside class=\"panel\"><h1>@TLALPOWA_BUILD_PROJECT_NAME@</h1><p>Facsímil estático generado durante la compilación.</p><p>@TLALPOWA_BUILD_TIMESTAMP@</p></aside></section><script src=\"main.js\"></script></main></body></html>\n")
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
set(TLALPOWA_CSS_FALLBACK "body{margin:0;font-family:Segoe UI,Arial,sans-serif;background:#eef4fa;color:#1d2835}.toolbar{height:58px;background:#f7fbff;border-bottom:1px solid #b8c9d8;display:flex;align-items:end;gap:8px;padding:0 12px}.tab{padding:10px 24px;border:1px solid #b2c2d2;border-bottom-color:#fff;border-radius:13px 13px 0 0;background:#fff}.command{margin-left:auto;background:#f10a6d;color:white;border:0;border-radius:10px;padding:9px 18px}.workspace{height:calc(100vh - 58px);display:grid;grid-template-columns:1fr 360px}.map{background:radial-gradient(circle at 50% 42%,#9f9273,#657c5d 35%,#b99d72 70%,#607849);position:relative}.label{position:absolute;left:50%;top:50%;transform:translate(-50%,-50%);padding:10px 18px;background:#ffffffc8;border-radius:999px}.panel{background:#f7fbff;border-left:1px solid #c2d0dc;padding:18px}\n")
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
set(TLALPOWA_JS_FALLBACK "console.info('Tlalpowa facsímil estático generado durante la compilación');\n")

tlalpowa_read_or_fallback(TLALPOWA_INDEX_TEXT "${TLALPOWA_INDEX_TEMPLATE}" "${TLALPOWA_INDEX_FALLBACK}")
tlalpowa_read_or_fallback(TLALPOWA_CSS_TEXT "${TLALPOWA_CSS_TEMPLATE}" "${TLALPOWA_CSS_FALLBACK}")
tlalpowa_read_or_fallback(TLALPOWA_JS_TEXT "${TLALPOWA_JS_TEMPLATE}" "${TLALPOWA_JS_FALLBACK}")


# Bloque function: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
function(tlalpowa_write_if_different output_path content)
  get_filename_component(_dir "${output_path}" DIRECTORY)
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(_dir)
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(MAKE_DIRECTORY "${_dir}")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(_tmp "${output_path}.tmp")
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  file(WRITE "${_tmp}" "${content}")
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  set(_replace TRUE)
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(EXISTS "${output_path}")
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(SHA256 "${output_path}" _old_hash)
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(SHA256 "${_tmp}" _new_hash)
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    if(_old_hash STREQUAL _new_hash)
# Bloque set: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
      set(_replace FALSE)
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    endif()
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(_replace)
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(REMOVE "${output_path}")
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(RENAME "${_tmp}" "${output_path}")
# Bloque else: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  else()
# Bloque file: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    file(REMOVE "${_tmp}")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
endfunction()

# Bloque foreach: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
foreach(_dir IN LISTS TLALPOWA_WEB_OUTPUT_DIR TLALPOWA_WEB_RUNTIME_OUTPUT_DIR)
# Bloque if: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  if(NOT "${_dir}" STREQUAL "")
    tlalpowa_write_if_different("${_dir}/index.html" "${TLALPOWA_INDEX_TEXT}")
    tlalpowa_write_if_different("${_dir}/main.css" "${TLALPOWA_CSS_TEXT}")
    tlalpowa_write_if_different("${_dir}/main.js" "${TLALPOWA_JS_TEXT}")
# Bloque message: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
    message(STATUS "facsimil web estatico generado en: ${_dir}")
# Bloque endif: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
  endif()
# Bloque endforeach: se mantiene separado para que la intención de configuración sea auditable y para evitar que rutas, opciones, dependencias o banderas terminen acopladas al código de runtime.
endforeach()
