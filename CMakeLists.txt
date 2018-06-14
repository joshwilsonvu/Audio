cmake_minimum_required(VERSION 3.6)
project(Audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -pedantic -pedantic-errors")

# Include project headers
include_directories(./include)

set(SOURCE_FILES
        src/AudioFile.cpp
        src/main.cpp
        src/ProcessCommands/ChangeGain.cpp
        src/ProcessCommands/Compressor.cpp
        src/Factory.cpp
        src/ProcessCommands/Filter.cpp
        src/Helpers.cpp
        src/Interpreter.cpp
        src/ProcessCommands/ProcessCommandMacro.cpp
        )

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(Audio ${SOURCE_FILES})