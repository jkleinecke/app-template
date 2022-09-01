cmake_minimum_required(VERSION 3.23)
project(titan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)


set(APP_FILES
   src/os/test.cpp
)

add_executable(app ${APP_FILES})
