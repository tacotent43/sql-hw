cmake_minimum_required(VERSION 3.25.1)
project(sqlab LANGUAGES CXX)

include(FetchContent)

option(USE_SOCI "Build with SOCI + Postgres support" OFF)
option(USE_TLS "Enable Transport Layer Security (OpenSSL)" OFF)
option(USE_CLICKHOUSE "Build with clickhouse native support" ON)

if(NOT USE_SOCI AND NOT USE_CLICKHOUSE)
    message(WARNING "No options chosen, enabling both SOCI and clickhouse")
    set(USE_SOCI ON)
    set(USE_CLICKHOUSE ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sqlab main.cpp)

# target_compile_options(sqlab PRIVATE
#     -Wall -Wextra -pedantic -g -fsanitize=address
# )

if(USE_SOCI)
    FetchContent_Declare(
        soci
        GIT_REPOSITORY https://github.com/SOCI/soci.git
        GIT_TAG v4.1.1
    )
    set(SOCI_POSTGRESQL ON CACHE BOOL "" FORCE)
    set(SOCI_STATIC ON CACHE BOOL "" FORCE)
    set(SOCI_SHARED OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(soci)

    file(GLOB SOCI_SRC soci-postgres/src/*.cpp)

    target_sources(sqlab PRIVATE ${SOCI_SRC})
    target_include_directories(sqlab PRIVATE soci-postgres/include)
    target_link_libraries(sqlab PRIVATE soci_core soci_postgresql)
    target_compile_definitions(sqlab PRIVATE HAVE_SOCI)
endif()

# if(USE_CLICKHOUSE)
#     set(WITH_OPENSSL ${USE_TLS} CACHE BOOL "Enable OpenSSL in clickhouse-cpp" FORCE)

#     FetchContent_Declare(
#         clickhouse-cpp
#         GIT_REPOSITORY https://github.com/ClickHouse/clickhouse-cpp.git
#         GIT_TAG v2.6.0
#     )
#     FetchContent_MakeAvailable(clickhouse-cpp)

#     file(GLOB CLICKHOUSE_SRC clickhouse-impl/src/*.cpp)

#     target_sources(sqlab PRIVATE ${CLICKHOUSE_SRC})
#     target_include_directories(sqlab PRIVATE clickhouse-impl/include)
#     target_link_libraries(sqlab PRIVATE clickhouse-cpp)
#     target_compile_definitions(sqlab PRIVATE HAVE_CLICKHOUSE)
# endif()

if(USE_CLICKHOUSE)
    set(WITH_OPENSSL ${USE_TLS} CACHE BOOL "Enable OpenSSL in clickhouse-cpp" FORCE)

    FetchContent_Declare(
        clickhouse-cpp
        GIT_REPOSITORY https://github.com/ClickHouse/clickhouse-cpp.git
        GIT_TAG v2.6.0
    )
    FetchContent_MakeAvailable(clickhouse-cpp)

    file(GLOB CLICKHOUSE_SRC clickhouse-impl/src/*.cpp)

    target_sources(sqlab PRIVATE ${CLICKHOUSE_SRC})
    target_include_directories(sqlab PRIVATE
        clickhouse-impl/include
        ${clickhouse-cpp_SOURCE_DIR}/include
    )
    target_link_libraries(sqlab PRIVATE clickhouse-cpp-lib)
    target_compile_definitions(sqlab PRIVATE HAVE_CLICKHOUSE)
endif()

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_link_libraries(sqlab PRIVATE nlohmann_json::nlohmann_json)