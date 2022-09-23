cmake_minimum_required(VERSION 3.23)
project(titan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    # 
    add_compile_options(/W4 /WX /wd4131 /wd4310 /wd5195 /wd4042 /wd4127 /wd4067 /wd4100 /wd4189 /wd5105 /wd4505)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-unused-function)
endif()

if(WIN32)
   set(CMAKE_WIN32_EXECUTABLE ON)
   set(OS_LIBS Winmm.lib Userenv.lib opengl32.lib)
elseif(APPLE)
   set(CMAKE_MACOSX_BUNDLE ON)
   set(OS_LIBS )
else()
   set(OS_LIBS )
endif()

set(APP_FILES
   # src/app.cpp
   src/os/test.cpp
)

add_executable(app ${APP_FILES})

target_link_libraries(app ${OS_LIBS})
