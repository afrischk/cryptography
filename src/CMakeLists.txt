cmake_minimum_required(VERSION 3.23)

project(craxor)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX d)


set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADER_FILES
  ${INCLUDE_DIR}/xor.h
  ${INCLUDE_DIR}/base64.h
  ${INCLUDE_DIR}/score.h
  ${INCLUDE_DIR}/algorithms.h
  ${INCLUDE_DIR}/io.h
  ${INCLUDE_DIR}/test/set-1-test.h
)

set(SOURCES
  main/craxor.c
  test/set-1-test.c
  xor.c
  base64.c
  score.c
  algorithms.c
  io.c
  ${HEADER_FILES}
)

set(WARNING_FLAGS
  -Wall
  -Wextra
  -Wpedantic
  -Wformat=2
  -Wno-unused-parameter
  -Wshadow
  -Wwrite-strings
  -Wstrict-prototypes
  -Wold-style-definition
  -Wredundant-decls
  -Wnested-externs
  -Wmissing-include-dirs
)

set(FLAGS
   -fsanitize=address
)

# add the executable
add_executable(craxor ${SOURCES})
target_link_libraries(craxor m)
target_compile_options(craxor PRIVATE ${WARNING_FLAGS} ${FLAGS})
target_link_options(craxor PRIVATE ${FLAGS})
target_include_directories(craxor PRIVATE ${INCLUDE_DIR} ${INCLUDE_DIR}/test)
set_target_properties(craxor PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} C_STANDARD 11)
