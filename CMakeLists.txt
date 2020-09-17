cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.18)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.18)
endif()

project(TestENTT  VERSION 0.1.0
                  DESCRIPTION "Test entt and other libraries"
                  LANGUAGES CXX)

add_executable(test_entt src/main.cpp)
