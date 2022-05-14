cmake_minimum_required(VERSION 3.10)
project(WitchHutFinder LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We need to build Cubiomes as a dependency
add_custom_target(
   cubiomes
   COMMAND make
   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/cubiomes"
)

# add the executable
add_executable(WitchHutFinder finder.cpp)

if (OPTIMIZE_SEED)
    target_compile_definitions(WitchHutFinder PRIVATE -DOPTIMIZE_SEED=${OPTIMIZE_SEED})
endif()

target_include_directories(WitchHutFinder PRIVATE "${CMAKE_SOURCE_DIR}/cubiomes")
target_link_libraries(WitchHutFinder PUBLIC -lm -fwrapv -lpthread)
target_link_libraries(WitchHutFinder PUBLIC ${CMAKE_SOURCE_DIR}/cubiomes/libcubiomes.a)

# Add the cubiomes dependency to ensure it is built.
add_dependencies(WitchHutFinder cubiomes)
