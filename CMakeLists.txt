cmake_minimum_required(VERSION 3.20)

project(CppDiplomProject)


set(OPENSSL_ROOT_DIR "C:\\Program Files\\OpenSSL-Win64")

set(BOOST_ROOT "C:\\Users\\Lygush\\Desktop\\SoftWare\\boost")

set(libpqxx_DIR "E:\\Stady\\Netology_cpp\\HomeWorks\\9_Diploma\\libpqxx-7.8.1")


# Boost:
find_package(Boost 1.80.0 REQUIRED COMPONENTS system thread locale)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost")
endif()


# PQXX
set(PostgreSQL_ADDITIONAL_VERSIONS "16")

add_subdirectory(${libpqxx_DIR} libpqxx-build) 


# OpenSSL
find_package(OpenSSL REQUIRED)

add_subdirectory(spider)

add_subdirectory(http_server)
