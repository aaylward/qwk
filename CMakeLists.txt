cmake_minimum_required(VERSION 3.20)

project(qwk VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS program_options REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_compile_options(-Wall -Wextra -Werror -Wshadow -pedantic)
add_executable(qwk ${SOURCES})
target_link_libraries(qwk Boost::program_options)

install(TARGETS qwk DESTINATION bin)
