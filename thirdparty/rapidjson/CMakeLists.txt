# Header-only RapidJSON library
# Provides the rapidjson::rapidjson INTERFACE target

add_library(rapidjson INTERFACE)
add_library(rapidjson::rapidjson ALIAS rapidjson)

target_include_directories(rapidjson INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

