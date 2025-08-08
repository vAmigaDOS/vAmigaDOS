# Main CMake file for vAmigaDOS (WASM build)
# Usage: emcmake cmake -S . -B <builddir>

cmake_minimum_required(VERSION 3.16)

# Language options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project setup
project(vAmiga)
add_executable(vAmiga main.cpp Proxy.cpp)

# Emscripten compiler settings
target_compile_options(vAmiga PRIVATE -O3)
target_compile_options(vAmiga PRIVATE -sWASM_WORKERS)
target_compile_options(vAmiga PRIVATE -Wall -Werror -Wfatal-errors)
target_compile_options(vAmiga PRIVATE -Wno-unused-parameter)
target_compile_options(vAmiga PRIVATE -mnontrapping-fptoint -fwasm-exceptions)

# Add include path for VAmiga.h
target_include_directories(vAmiga PRIVATE
    Core
    Core/Components
    Core/Components/Agnus
    Core/Components/Agnus/Blitter
    Core/Components/Agnus/Copper
    Core/Components/Agnus/DmaDebugger
    Core/Components/Agnus/Sequencer
    Core/Components/CIA
    Core/Components/CPU
    Core/Components/Denise
    Core/Components/Memory
    Core/Components/Paula
    Core/Components/Paula/Audio
    Core/Components/Paula/DiskController
    Core/Components/Paula/UART
    Core/Components/RTC
    Core/Components/Zorro
    Core/FileSystems
    Core/Infrastructure
    Core/Media
    Core/Misc
    Core/Peripherals
    Core/Peripherals/Drive
    Core/Peripherals/Joystick
    Core/Peripherals/Keyboard
    Core/Peripherals/Monitor
    Core/Peripherals/Mouse
    Core/Ports
    Core/Utilities
)

add_subdirectory("Core")
target_link_libraries(vAmiga VACore)

# Emscripten linker settings
set(LF_MEM "-sINITIAL_MEMORY=512MB -sTOTAL_STACK=32MB -sALLOW_MEMORY_GROWTH=0")
set(LF_ENV "-sENVIRONMENT='web,worker'")
set(LF_MOD "-sMODULARIZE=1 -sEXPORT_ES6=1 -sEXPORT_ALL=1")
set(LF_OPT "-O1 -mnontrapping-fptoint -fwasm-exceptions")
set(LF_EXPORT "-lembind -sEXPORT_EXCEPTION_HANDLING_HELPERS")
set(LF_THREAD "-pthread -sPTHREAD_POOL_SIZE=1 -sWASM_WORKERS")
set(LF_DEBUG "-sASSERTIONS=0 -sGL_ASSERTIONS=0")
set(LF_OTHER "-sWASM_BIGINT -sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 -sLLD_REPORT_UNDEFINED")
set_target_properties(vAmiga PROPERTIES LINK_FLAGS "${LF_MEM} ${LF_ENV} ${LF_MOD} ${LF_OPT} ${LF_EXPORT} ${LF_THREAD} ${LF_DEBUG} ${LF_OTHER}" )

set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(PUBLISH_FOLDER "${CMAKE_SOURCE_DIR}/../src/lib")

add_custom_command(TARGET vAmiga POST_BUILD
    COMMAND cp vAmiga.* ${PUBLISH_FOLDER}
)
