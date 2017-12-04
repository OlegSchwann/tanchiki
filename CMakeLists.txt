cmake_minimum_required(VERSION 3.8)
project(simple_RTS)

set(CMAKE_VERBOSE_MAKEFILE ON) # подробный лог.

set(CMAKE_CXX_STANDARD 17)

# в ручную добавляем пути
# библиотеки линкуются компилятором при опции -l 
# $ g++ 1.cpp -lm libm.so -L~/usr/lib -luserlib libuserlib.so
include_directories(/home/oleg/CLionProject/simple_RTS/SFML-2.4.2/include)
link_libraries(/home/oleg/CLionProject/simple_RTS/SFML-2.4.2/lib)

set(SOURCE_FILES main.cpp DrawScene.cpp DrawScene.h AbstractScene.cpp AbstractScene.h InterfaseScene.cpp InterfaseScene.h PhisicalScene.cpp PhisicalScene.h ControllerScene.cpp ControllerScene.h PleerController.cpp PleerController.h)
add_executable(simple_RTS ${SOURCE_FILES})
target_link_libraries(simple_RTS sfml-system sfml-graphics sfml-window sfml-network)
