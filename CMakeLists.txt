cmake_minimum_required(VERSION 3.15)
project(btc-lottery-miner VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Find nlohmann_json
find_package(nlohmann_json REQUIRED)

add_executable(btc-lottery-miner
    src/btcclient.cpp
    src/configfile.cpp
    src/main.cpp)

# Link libraries
target_link_libraries(btc-lottery-miner
    PRIVATE
    Boost::system
    cppcodec
    nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(btc-lottery-miner
    PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cppcodec
)

# Add options for warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(btc-lottery-miner PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(btc-lottery-miner PRIVATE /W4 /WX)
endif()

