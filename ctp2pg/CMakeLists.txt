cmake_minimum_required(VERSION 3.23)
project(ctp2pg)

set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

find_package(libpqxx CONFIG REQUIRED)

add_library(ctp2pg_ STATIC ctp2pg.cpp)
target_include_directories(ctp2pg_ PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(ctp2pg_ PUBLIC ctp_wrap libpqxx::pqxx)

find_package(CLI11 CONFIG REQUIRED)
add_executable(ctp2pg main.cpp)
if (NOT MSVC)
    target_link_libraries(ctp2pg PRIVATE ctp2pg_ CLI11::CLI11)
    set_target_properties(ctp2pg PROPERTIES
            LINK_FLAGS "-Wl,-rpath,lib")
endif ()
