cmake_minimum_required(VERSION 3.23)

# set the project name and version
project(xor-cipher)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX d)


set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HEADER_FILES
  ${INCLUDE_DIR}/xor-crack.h
  ${INCLUDE_DIR}/xor-crack-types.h
  ${INCLUDE_DIR}/xor-crack-repeat.h
  ${INCLUDE_DIR}/xor-detect-single.h
  ${INCLUDE_DIR}/xor-encrypt.h
  ${INCLUDE_DIR}/xor-encrypt-repeat.h
  ${INCLUDE_DIR}/base64.h
  ${INCLUDE_DIR}/score.h
  ${INCLUDE_DIR}/algorithms.h
  ${INCLUDE_DIR}/io.h
  ${INCLUDE_DIR}/test/set-1-test.h
)

set(SOURCES
  main/xor-crack-repeat-main.c
  test/set-1-test.c
  xor-crack-repeat.c
  xor-crack.c
  xor-detect-single.c
  xor-encrypt.c
  xor-encrypt-repeat.c
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
add_executable(xor-cipher ${SOURCES})
target_link_libraries(xor-cipher m)
target_compile_options(xor-cipher PRIVATE ${WARNING_FLAGS} ${FLAGS})
target_link_options(xor-cipher PRIVATE ${FLAGS})
target_include_directories(xor-cipher PRIVATE ${INCLUDE_DIR} ${INCLUDE_DIR}/test)
set_target_properties(xor-cipher PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} C_STANDARD 11)
