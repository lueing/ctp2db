cmake_minimum_required(VERSION 3.23)
project(ctp2db)

set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

if (MSVC)
    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif ()

if (CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4996 /wd4702 /wd4100")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -O3")
    endif ()
    message(STATUS "Building release version, flags : ${CMAKE_CXX_FLAGS}...")
else ()
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4996 /wd4702 /wd4100")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -O0")
    endif ()
    message(STATUS "Building debug version, flags : ${CMAKE_CXX_FLAGS}...")
endif ()

add_subdirectory(ctp-wrapper)
add_subdirectory(ctp2pg)

install(TARGETS ctp2pg
        RUNTIME DESTINATION bin)
install(FILES ctp-wrapper/ctp-wrapper/config-sample.yaml
        DESTINATION .)

if (MSVC)
    install(FILES
            $<TARGET_RUNTIME_DLLS:ctp2pg>
            ctp-wrapper/ctp-wrapper/lib/v672/win64/thostmduserapi_se.dll
            ctp-wrapper/ctp-wrapper/lib/v672/win64/thosttraderapi_se.dll
            DESTINATION bin
            )
else ()
    install(FILES ctp-wrapper/ctp-wrapper/lib/v672/linux64/thostmduserapi_se.so
            ctp-wrapper/ctp-wrapper/lib/v672/linux64/thosttraderapi_se.so
            DESTINATION lib)
endif ()

set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/.idea/;CMakeUserPresets.json")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "ctp2pg")
set(CPACK_PACKAGE_VERSION "0.0.1")
include(CPack)