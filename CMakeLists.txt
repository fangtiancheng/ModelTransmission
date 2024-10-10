cmake_minimum_required(VERSION 3.15)
project(ModelTransmission)

find_package(CURL REQUIRED)

# add library

# test main
add_compile_definitions(TEST_FILE_LOC="${CMAKE_CURRENT_SOURCE_DIR}/resources/color.png")
#add_compile_definitions(TEST_FILE_LOC="${CMAKE_CURRENT_SOURCE_DIR}/resources/1G.file")

# http server
add_executable(main-http-server test/http-server.cpp)
target_include_directories(main-http-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# http client
add_executable(main-http-client test/http-client.cpp)
target_include_directories(main-http-client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
