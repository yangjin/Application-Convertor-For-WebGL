cmake_minimum_required(VERSION 2.8)
project(W2N_GBM_ES2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/run)
set(CMAKE_BUILD_TYPE Debug)

include_directories("${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/lib"
					"${PROJECT_SOURCE_DIR}/lib/include"
					"${PROJECT_SOURCE_DIR}/lib/include/core"
					"${PROJECT_SOURCE_DIR}/lib/include/config"
					"${PROJECT_SOURCE_DIR}/lib/include/codec")
link_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(lib)

include_directories("${PROJECT_SOURCE_DIR}/w2n")
add_subdirectory(w2n)
