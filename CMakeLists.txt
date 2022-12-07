cmake_minimum_required(VERSION 3.16)
project(fake_essentials)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

link_directories(${CMAKE_SOURCE_DIR}/lib)
find_package( CURL REQUIRED )

add_library( discord_game_sdk SHARED IMPORTED )
set_target_properties( discord_game_sdk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/discord_game_sdk${CMAKE_SHARED_LIBRARY_SUFFIX} )

FILE(GLOB DISCORD_CPP_SOURCES discord/*)
add_executable(fake_essentials
        main.cpp
        ${DISCORD_CPP_SOURCES} json/json.hpp utils.cpp utils.h argparse.cpp argparse.h)
add_link_options(--verbose)

target_link_libraries(fake_essentials discord_game_sdk)
target_link_libraries( fake_essentials CURL::libcurl )
