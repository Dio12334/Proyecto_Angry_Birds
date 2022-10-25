cmake_minimum_required(VERSION 3.16.3)
project(Proyecto_Angry_Birds)

set(CMAKE_CXX_STANDARD 17)

file(GLOB helloworld_SRC
        "include/*.h"
        "src/*.cpp"
        )

add_executable(Proyecto_Angry_Birds main.cpp ${helloworld_SRC})

target_link_libraries(Proyecto_Angry_Birds -lSDL2 -lSOIL -lGLEW  -lGL -lGLU -lGL -lm -lX11 )