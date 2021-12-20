cmake_minimum_required(VERSION 3.10)
project(WitchHutFinder LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add the executable
add_executable(WitchHutFinder finder.cpp)

target_compile_definitions(WitchHutFinder PRIVATE -DSEED_USED=${SEED_USED})

target_include_directories(WitchHutFinder PRIVATE "${CMAKE_SOURCE_DIR}/cubiomes")
target_link_libraries(WitchHutFinder PUBLIC -lm -fwrapv)
target_link_libraries(WitchHutFinder PUBLIC ${CMAKE_SOURCE_DIRECTORY}/cubiomes/libcubiomes.a)
