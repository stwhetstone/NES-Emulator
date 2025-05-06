cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(nes-emulator)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Create your game executable target as usual
add_executable(main src/main.cpp src/NES.cpp src/CPU.cpp src/ROM.cpp)
