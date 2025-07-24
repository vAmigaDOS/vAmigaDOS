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
    emulator
    emulator/Components
    emulator/Components/Agnus
    emulator/Components/Agnus/Blitter
    emulator/Components/Agnus/Copper
    emulator/Components/Agnus/DmaDebugger
    emulator/Components/Agnus/Sequencer
    emulator/Components/CIA
    emulator/Components/CPU
    emulator/Components/Denise
    emulator/Components/Memory
    emulator/Components/Paula
    emulator/Components/Paula/Audio
    emulator/Components/Paula/DiskController
    emulator/Components/Paula/UART
    emulator/Components/RTC
    emulator/Components/Zorro
    emulator/FileSystems
    emulator/Infrastructure
    emulator/Media
    emulator/Misc
    emulator/Peripherals
    emulator/Peripherals/Drive
    emulator/Peripherals/Joystick
    emulator/Peripherals/Keyboard
    emulator/Peripherals/Monitor
    emulator/Peripherals/Mouse
    emulator/Ports
    emulator/Utilities
)

add_subdirectory("emulator")
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
