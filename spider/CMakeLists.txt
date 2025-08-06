cmake_minimum_required(VERSION 3.20)

add_executable(SpiderApp
    main.cpp
    http_utils.h
    http_utils.cpp
    link.h
    html_parser.h
    html_parser.cpp
    spider_database_worker.h
    spider_database_worker.cpp
    ../common/ini_parser.h
    ../common/ini_parser.cpp
    ../common/common_functions.h
    ../common/common_functions.cpp
    ../common/database_worker.h
    ../common/database_worker.cpp
    ../common/word_worker.h
    ../common/word_worker.cpp
)

target_compile_features(SpiderApp PRIVATE cxx_std_17)

# Линковка библиотек
target_link_libraries(SpiderApp
    pqxx
    OpenSSL::SSL
    ${Boost_LIBRARIES}
)

# Инклуды
target_include_directories(SpiderApp PRIVATE
    ${Boost_INCLUDE_DIRS}
    ../common
)

# Копирование ini-файла
add_custom_command(
    TARGET SpiderApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/Properties.ini
            ${CMAKE_CURRENT_BINARY_DIR}
)
