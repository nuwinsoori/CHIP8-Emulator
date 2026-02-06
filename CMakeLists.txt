cmake_minimum_required(VERSION 3.16)
project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chip8
  src/main.cpp
  src/cpu.cpp
)

target_include_directories(chip8 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(chip8 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}
)

find_package(SDL3 CONFIG QUIET)

if(SDL3_FOUND)
  target_link_libraries(chip8 PRIVATE SDL3::SDL3)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED sdl3)

  target_include_directories(chip8 PRIVATE ${SDL3_INCLUDE_DIRS})
  target_link_libraries(chip8 PRIVATE ${SDL3_LIBRARIES})
  target_compile_options(chip8 PRIVATE ${SDL3_CFLAGS_OTHER})
endif()

if(MSVC)
  target_compile_options(chip8 PRIVATE /W4)
else()
  target_compile_options(chip8 PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS chip8 RUNTIME DESTINATION bin)
