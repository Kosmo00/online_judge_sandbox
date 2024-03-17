cmake_minimum_required(VERSION 3.0.0)
project(coj_sandbox VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE sources "src/*.cpp")

file(COPY input DESTINATION ${CMAKE_BINARY_DIR})
file(COPY output DESTINATION ${CMAKE_BINARY_DIR})
file(COPY submissions DESTINATION ${CMAKE_BINARY_DIR})

add_executable(sandbox ${sources})

include_directories(include)

target_link_libraries(sandbox PRIVATE pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)