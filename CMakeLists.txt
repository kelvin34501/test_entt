cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.18)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.18)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

project(TestENTT  VERSION 0.1.0
                  DESCRIPTION "Test entt and other libraries"
                  LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/external/tclap)
set(TCLAP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tclap/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/entt)
set(ENTT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/entt/src)

add_executable(test_entt src/main.cpp)
target_compile_features(test_entt PRIVATE
    cxx_std_17
)
target_include_directories(test_entt PRIVATE
    ${ENTT_INCLUDE_DIR}
)