cmake_minimum_required(VERSION 3.10)
project("Selena compiler")


add_compile_options(
  -std=gnu17
  -pedantic
  -Wall -Wextra --pedantic-errors
  -Wconversion -Wsign-conversion
  -Wshadow -Wfloat-equal
  -Wformat=2 -Wformat-security
  -Wcast-align -Wcast-qual
  -Wmissing-include-dirs
  -Wduplicated-cond -Wduplicated-branches
  -Wrestrict
  -Wstringop-overflow=4
  -Wlogical-op
  -Wnonnull
  -Wstack-protector
  -Werror

# -fanalyzer

  -O2
  -Ofast
  -march=native -mtune=native
  -flto
  -fuse-linker-plugin
  -ffat-lto-objects
  -fstrict-aliasing
  -fomit-frame-pointer
  -funroll-loops -funswitch-loops
  -fprefetch-loop-arrays

# -flto-partition=balanced
  -DNDEBUG
  -fstack-clash-protection
  -fno-common
  -fdiagnostics-color=always
)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto -fuse-linker-plugin")


add_executable(selena)

target_include_directories(selena PUBLIC
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/
)
target_sources(selena PRIVATE 
    resources/msg_resource.c
    src/utils/allocation.c
    src/utils/cli_colors.c
    src/utils/msg_errors.c
    src/selena.c
    src/main.c
)