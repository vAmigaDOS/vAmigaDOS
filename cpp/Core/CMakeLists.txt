cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Set the project name
project(VACore)

# Enable testing
list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
enable_testing()

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# Set the optimization level (if not specified on the command line)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Find and optionally link zlib
find_package(ZLIB QUIET)
if(ZLIB_FOUND)
  message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES}")
else()
  message(STATUS "ZLIB not found. Compression support will be disabled.")
endif()

# Add the emulator library
add_library(VACore VAmiga.cpp config.cpp)

# Specify compile options
target_compile_definitions(VACore PUBLIC _USE_MATH_DEFINES)
target_compile_options(VACore PRIVATE -Wall -Werror)
target_compile_options(VACore PRIVATE -Wno-unused-parameter -Wno-unused-but-set-variable)
target_compile_options(VACore PRIVATE -O3 -mnontrapping-fptoint -fwasm-exceptions)
target_compile_options(VACore PRIVATE -Wfatal-errors)
target_compile_options(VACore PRIVATE -sWASM_WORKERS)
set_target_properties(VACore PROPERTIES LINK_FLAGS "-O1 -mnontrapping-fptoint -fwasm-exceptions" )

# Define preprocessor macros
add_compile_definitions(VAMIGA_DOS)

# Add include paths
target_include_directories(VACore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add subdirectories
add_subdirectory(Infrastructure)
add_subdirectory(Components)
add_subdirectory(FileSystems)
add_subdirectory(Media)
add_subdirectory(Misc)
add_subdirectory(Peripherals)
add_subdirectory(Ports)
add_subdirectory(ThirdParty)
add_subdirectory(Utilities)

# Add libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(VACore xdms Threads::Threads)

# Link zlib if available
if(ZLIB_FOUND)
  target_link_libraries(VACore ZLIB::ZLIB)
  target_compile_definitions(VACore PUBLIC USE_ZLIB=1)  # Optional define for conditional compilation
endif()

target_link_libraries(VACore lz4)
