cmake_minimum_required(VERSION 3.16)
project(fake_essential)

set(CMAKE_CXX_STANDARD 14)

IF(!WIN32)
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
ENDIF() # IF(!WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

IF (WIN32)
  find_package(CURL CONFIG REQUIRED)
ELSE()
  find_package( CURL REQUIRED )
ENDIF() #IF (WIN32)

add_library( discord_game_sdk SHARED IMPORTED )

IF (WIN32)
  set_target_properties( discord_game_sdk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/discord_game_sdk.dll.lib )
  set_target_properties( discord_game_sdk PROPERTIES IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/lib/discord_game_sdk.dll.lib )
ELSE()
  set_target_properties( discord_game_sdk PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/discord_game_sdk${CMAKE_SHARED_LIBRARY_SUFFIX} )
ENDIF() # IF WIN32


FILE(GLOB DISCORD_CPP_SOURCES ${CMAKE_SOURCE_DIR}/discord/*)
add_executable(fake_essential
        main.cpp
        ${DISCORD_CPP_SOURCES} json/json.hpp utils.cpp utils.h argparse.cpp argparse.h)

target_link_libraries( fake_essential PRIVATE CURL::libcurl discord_game_sdk)

set(
        ZIP_SRC
        "${CMAKE_SOURCE_DIR}/lib/discord_game_sdk.dll"
        "${CMAKE_BINARY_DIR}/fake_essential.exe"
        "${CMAKE_BINARY_DIR}/libcurl-d.dll"
        "${CMAKE_BINARY_DIR}/zlibd1.dll"
)
IF(WIN32)
  add_custom_target(create_zip COMMAND
          ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/fake_essential.zip" --format=zip
          "${CMAKE_BINARY_DIR}/fake_essential.exe"
          "${CMAKE_BINARY_DIR}/libcurl-d.dll"
          "${CMAKE_BINARY_DIR}/zlibd1.dll"
          discord_game_sdk
          DEPENDS fake_essential discord_game_sdk)
  add_custom_target("build-zip" ALL DEPENDS "${CMAKE_BINARY_DIR}/fake_essential.zip")
ENDIF()
