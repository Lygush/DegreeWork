cmake_minimum_required(VERSION 3.20)
project(CppDiplomProject)

# Указываем C++17 для всего проекта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# -------------------------------------------------
# Boost
# -------------------------------------------------
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

find_package(Boost 1.74.0 COMPONENTS system thread locale REQUIRED)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

# -------------------------------------------------
# OpenSSL
# -------------------------------------------------
find_package(OpenSSL REQUIRED)
if(NOT OpenSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found")
endif()

# -------------------------------------------------
# libpqxx (PostgreSQL C++ client)
# -------------------------------------------------
find_package(PkgConfig)
pkg_check_modules(LIBPQXX libpqxx)

if (NOT LIBPQXX_FOUND)
    message(STATUS "libpqxx not found — будет загружен и собран вручную")
    FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG 7.8.1
    )
    FetchContent_MakeAvailable(libpqxx)
endif()

# -------------------------------------------------
# Subdirectories
# -------------------------------------------------
add_subdirectory(spider)
add_subdirectory(http_server)