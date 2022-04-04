cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 20)

project(logging-tests)

include_directories(
	spdlog/include
)

add_executable( ${PROJECT_NAME}
  src/main.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

target_link_libraries(${PROJECT_NAME}
	pthread
)
