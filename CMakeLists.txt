cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_CONFIGURATION Release CACHE STRING "Use Release try-compile checks to avoid slow MSVC probes" FORCE)
project(Tlalpowa LANGUAGES C CXX)

set(CMAKE_SUPPRESS_REGENERATION ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(TLALPOWA_ENABLE_IMGUI "Build the portable Dear ImGui/GLFW map application" ON)
option(TLALPOWA_ENABLE_LTCG "Enable MSVC whole-program/LTCG optimization; slower and heavier builds" OFF)
option(TLALPOWA_ENABLE_CL_MP "Enable MSVC /MP compiler fan-out; faster but heavier on the PC" OFF)

set(TLALPOWA_DEPS_ROOT "$ENV{TLALPOWA_DEPS_ROOT}" CACHE PATH "Global dependency root")
if(NOT TLALPOWA_DEPS_ROOT)
  set(TLALPOWA_DEPS_ROOT "C:/ProgramData/Tlalpowa/Dependencias" CACHE PATH "Global dependency root" FORCE)
endif()

set(TLALPOWA_RUNTIME_OUTPUT_DIR "" CACHE PATH "Directory where the final executable is written")
if(NOT TLALPOWA_RUNTIME_OUTPUT_DIR)
  if(WIN32)
    set(TLALPOWA_RUNTIME_OUTPUT_DIR "${CMAKE_SOURCE_DIR}")
  else()
    set(TLALPOWA_RUNTIME_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
  endif()
endif()
file(MAKE_DIRECTORY "${TLALPOWA_RUNTIME_OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TLALPOWA_RUNTIME_OUTPUT_DIR}")
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${TLALPOWA_RUNTIME_OUTPUT_DIR}")
endforeach()

set(TLALPOWA_CORE_SOURCES
  Fuente/Config.cpp
  Fuente/Dashboard.cpp
  Fuente/EpiTypes.cpp
  Fuente/ExternalTools.cpp
  Fuente/AtmosphereModel.cpp
  Fuente/GeneratedRuntimeResources.c
  Fuente/GuiLauncher.cpp
  Fuente/Launcher.cpp
  Fuente/core.c
  Fuente/main.cpp
  Fuente/TlalpowaApp.cpp
  Fuente/Ixiptlah.cpp
  Fuente/PdfExtractor.cpp
  Fuente/Pipeline.cpp
  Fuente/TableEngine.cpp
  Fuente/TemporalBlocks.cpp
  Fuente/TextUtils.cpp
  Fuente/Writers.cpp
)

set(TLALPOWA_ICON_PNG "${CMAKE_SOURCE_DIR}/tlalpowa.png")
set(TLALPOWA_ICON_ICO "${CMAKE_SOURCE_DIR}/Tlalpowa.ico")
set(TLALPOWA_ICON_CONVERTER "${CMAKE_SOURCE_DIR}/Fuente/Tools/Convert-PngToIco.ps1")

if(WIN32 AND EXISTS "${TLALPOWA_ICON_PNG}")
  find_program(TLALPOWA_POWERSHELL_EXE NAMES powershell.exe powershell)
  if(TLALPOWA_POWERSHELL_EXE AND EXISTS "${TLALPOWA_ICON_CONVERTER}")
    execute_process(
      COMMAND "${TLALPOWA_POWERSHELL_EXE}" -NoProfile -ExecutionPolicy Bypass
              -File "${TLALPOWA_ICON_CONVERTER}"
              -InputPng "${TLALPOWA_ICON_PNG}"
              -OutputIco "${TLALPOWA_ICON_ICO}"
      RESULT_VARIABLE TLALPOWA_ICON_RC
      OUTPUT_VARIABLE TLALPOWA_ICON_OUT
      ERROR_VARIABLE TLALPOWA_ICON_ERR
    )
    if(NOT TLALPOWA_ICON_RC EQUAL 0 OR NOT EXISTS "${TLALPOWA_ICON_ICO}")
      message(FATAL_ERROR "No pude convertir tlalpowa.png a Tlalpowa.ico. Salida: ${TLALPOWA_ICON_OUT} Error: ${TLALPOWA_ICON_ERR}")
    endif()
    configure_file("${TLALPOWA_ICON_ICO}" "${CMAKE_SOURCE_DIR}/Fuente/Tlalpowa.ico" COPYONLY)
    message(STATUS "Icono actualizado desde tlalpowa.png: ${TLALPOWA_ICON_OUT}")
  else()
    message(FATAL_ERROR "Existe tlalpowa.png, pero no encontre PowerShell o Fuente/Tools/Convert-PngToIco.ps1 para regenerar Tlalpowa.ico.")
  endif()
endif()

if(WIN32 AND EXISTS "${TLALPOWA_ICON_ICO}")
  list(APPEND TLALPOWA_CORE_SOURCES Fuente/app_icon.rc)
  set_source_files_properties(Fuente/app_icon.rc PROPERTIES LANGUAGE RC)
endif()

if(WIN32)
  add_executable(Tlalpowa WIN32 ${TLALPOWA_CORE_SOURCES})
else()
  add_executable(Tlalpowa ${TLALPOWA_CORE_SOURCES})
endif()

set_target_properties(Tlalpowa PROPERTIES OUTPUT_NAME "Tlalpowa")
target_include_directories(Tlalpowa PRIVATE Fuente)
if(EXISTS "${TLALPOWA_DEPS_ROOT}/include")
  target_include_directories(Tlalpowa PRIVATE "${TLALPOWA_DEPS_ROOT}/include")
endif()
if(WIN32)
  target_link_libraries(Tlalpowa PRIVATE Comctl32 Shell32 Urlmon Wininet)
endif()

# Tlalpowa parsea manifiestos JSON también fuera del modo gráfico: las nuevas
# importaciones web pueden resolver catálogos CKAN, OData, CMR y metadatos de
# descarga aun cuando se compile una variante mínima sin ImGui. Por eso
# nlohmann_json NO puede vivir dentro de TLALPOWA_ENABLE_IMGUI; hacerlo volvería
# a romper compilaciones de diagnóstico y, peor aún, induciría a retirar el
# parser de AtmosphereModel.cpp para “arreglar” la compilación, dando marcha
# atrás al flujo de materialización GeoJSON/JSON hacia núcleos IXIPTLAH
# mensuales. Esta dependencia es ligera y de cabecera, y queda deliberadamente
# enlazada al objetivo base para mantener una sola ruta de lectura robusta.
include(FetchContent)
find_package(nlohmann_json CONFIG QUIET)
if(NOT TARGET nlohmann_json::nlohmann_json)
  message(STATUS "nlohmann_json global no encontrado; usando FetchContent en Build/_deps.")
  FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()
target_link_libraries(Tlalpowa PRIVATE nlohmann_json::nlohmann_json)


# Los glifos externos colocados por el usuario en Datos/icon se normalizan en
# tiempo de compilacion antes de enlazar la aplicacion grafica. La normalizacion
# no depende de bitacoras ni manifiestos: el ejecutable auxiliar abre cada PNG,
# verifica si sus pixeles ya son exactamente blanco opaco o transparencia total,
# y solo reescribe cuando encuentra negro, gris o alfa intermedio. Esto impide
# bucles de reprocesamiento en compilaciones sucesivas y evita que Explorer, WIC
# o el renderizador vuelvan a mostrar halos grises por antialias heredado.
if(WIN32 AND EXISTS "${CMAKE_SOURCE_DIR}/Fuente/Tools/tlalpowa_icon_polarizer.cpp")
  add_executable(TlalpowaIconPolarizer Fuente/Tools/tlalpowa_icon_polarizer.cpp)
  target_link_libraries(TlalpowaIconPolarizer PRIVATE windowscodecs ole32)
  if(MSVC)
    target_compile_options(TlalpowaIconPolarizer PRIVATE /W4 /utf-8 /EHsc)
  else()
    target_compile_options(TlalpowaIconPolarizer PRIVATE -Wall -Wextra -pedantic)
  endif()
  add_custom_target(TlalpowaPolarizarIconos ALL
    COMMAND $<TARGET_FILE:TlalpowaIconPolarizer> --root "${CMAKE_SOURCE_DIR}"
    DEPENDS TlalpowaIconPolarizer
    COMMENT "Polarizando iconos de Datos/icon a blanco puro con fondo transparente"
    VERBATIM
  )
  add_dependencies(Tlalpowa TlalpowaPolarizarIconos)
endif()

if(TLALPOWA_ENABLE_IMGUI)
  if(WIN32)
    set(CMAKE_HAVE_LIBC_PTHREAD 0 CACHE INTERNAL "Windows build uses Win32 threads, not pthreads" FORCE)
    set(CMAKE_USE_WIN32_THREADS_INIT 1 CACHE INTERNAL "Windows build uses Win32 threads" FORCE)
    set(CMAKE_USE_PTHREADS_INIT 0 CACHE INTERNAL "Windows build does not use pthreads" FORCE)
    set(CMAKE_THREAD_LIBS_INIT "" CACHE STRING "Windows thread library is implicit" FORCE)
    set(THREADS_PREFER_PTHREAD_FLAG OFF CACHE BOOL "Prefer Win32 threads on Windows" FORCE)
  endif()
  include(FetchContent)
  find_package(OpenGL REQUIRED)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(TLALPOWA_GLFW_TARGET "")
  if(EXISTS "${TLALPOWA_DEPS_ROOT}/glfw/CMakeLists.txt")
    add_subdirectory("${TLALPOWA_DEPS_ROOT}/glfw" "${CMAKE_BINARY_DIR}/deps/glfw" EXCLUDE_FROM_ALL)
    set(TLALPOWA_GLFW_TARGET glfw)
  else()
    find_package(glfw3 CONFIG QUIET)
    if(TARGET glfw)
      set(TLALPOWA_GLFW_TARGET glfw)
    elseif(TARGET glfw3)
      set(TLALPOWA_GLFW_TARGET glfw3)
    endif()
  endif()
  if(NOT TLALPOWA_GLFW_TARGET)
    message(STATUS "GLFW global no encontrado; usando FetchContent en Build/_deps.")
    FetchContent_Declare(
      glfw
      URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    )
    FetchContent_MakeAvailable(glfw)
    set(TLALPOWA_GLFW_TARGET glfw)
  endif()
  if(EXISTS "$ENV{TLALPOWA_IMGUI_DIR}/imgui.cpp")
    set(imgui_SOURCE_DIR "$ENV{TLALPOWA_IMGUI_DIR}")
  elseif(EXISTS "${TLALPOWA_DEPS_ROOT}/imgui/imgui.cpp")
    set(imgui_SOURCE_DIR "${TLALPOWA_DEPS_ROOT}/imgui")
  else()
    message(STATUS "ImGui global no encontrado; usando FetchContent en Build/_deps.")
    FetchContent_Declare(
      imgui
      URL https://github.com/ocornut/imgui/archive/refs/heads/docking.zip
    )
    FetchContent_MakeAvailable(imgui)
  endif()
  set(TLALPOWA_STB_DIR "${CMAKE_BINARY_DIR}/_deps/stb")
  if(NOT EXISTS "${TLALPOWA_STB_DIR}/stb_image.h")
    file(MAKE_DIRECTORY "${TLALPOWA_STB_DIR}")
    message(STATUS "stb_image.h no encontrado; descargando en Build/_deps/stb.")
    file(DOWNLOAD
      "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
      "${TLALPOWA_STB_DIR}/stb_image.h"
      TLS_VERIFY ON
      STATUS TLALPOWA_STB_DOWNLOAD_STATUS
    )
    list(GET TLALPOWA_STB_DOWNLOAD_STATUS 0 TLALPOWA_STB_DOWNLOAD_CODE)
    if(NOT TLALPOWA_STB_DOWNLOAD_CODE EQUAL 0)
      message(FATAL_ERROR "No pude descargar stb_image.h; ejecuta Instalar_Dependencias_Globales_Tlalpowa.cmd o reintenta con internet.")
    endif()
  endif()
  target_include_directories(Tlalpowa PRIVATE "${TLALPOWA_STB_DIR}")
  add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
  target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
  target_link_libraries(imgui PUBLIC ${TLALPOWA_GLFW_TARGET} OpenGL::GL)
  target_compile_definitions(Tlalpowa PRIVATE TLALPOWA_ENABLE_IMGUI)
  target_link_libraries(Tlalpowa PRIVATE imgui ${TLALPOWA_GLFW_TARGET} OpenGL::GL)
endif()

if(WIN32 AND EXISTS "${TLALPOWA_DEPS_ROOT}/poppler/Library/bin/pdftotext.exe")
  add_custom_command(TARGET Tlalpowa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Tlalpowa>/Dependencias"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${TLALPOWA_DEPS_ROOT}/poppler"
      "$<TARGET_FILE_DIR:Tlalpowa>/Dependencias/poppler"
    COMMENT "Empaquetando Poppler junto a Tlalpowa.exe"
  )
  if(EXISTS "${TLALPOWA_DEPS_ROOT}/dependencies.local.json")
    add_custom_command(TARGET Tlalpowa POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${TLALPOWA_DEPS_ROOT}/dependencies.local.json"
        "$<TARGET_FILE_DIR:Tlalpowa>/Dependencias/dependencies.local.json"
      COMMENT "Copiando manifiesto local de dependencias"
    )
  endif()
endif()

if(MSVC)
  target_compile_options(Tlalpowa PRIVATE /W4 /utf-8)
  if(TLALPOWA_ENABLE_CL_MP)
    target_compile_options(Tlalpowa PRIVATE /MP)
  endif()
  target_compile_options(Tlalpowa PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/permissive-> $<$<COMPILE_LANGUAGE:CXX>:/Ob2> $<$<COMPILE_LANGUAGE:CXX>:/bigobj>)
  target_compile_options(Tlalpowa PRIVATE $<$<COMPILE_LANGUAGE:C>:/TC>)
  target_compile_options(Tlalpowa PRIVATE $<$<CONFIG:Release>:/O2> $<$<CONFIG:Release>:/Oi> $<$<CONFIG:Release>:/Ot> $<$<CONFIG:Release>:/Gy> $<$<CONFIG:Release>:/Gw>)
  target_link_options(Tlalpowa PRIVATE $<$<CONFIG:Release>:/OPT:REF> $<$<CONFIG:Release>:/OPT:ICF>)
  if(TLALPOWA_ENABLE_LTCG)
    target_compile_options(Tlalpowa PRIVATE $<$<CONFIG:Release>:/GL>)
    target_link_options(Tlalpowa PRIVATE $<$<CONFIG:Release>:/LTCG>)
  endif()
else()
  target_compile_options(Tlalpowa PRIVATE -Wall -Wextra -pedantic $<$<CONFIG:Release>:-O3>)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tlalpowa)
