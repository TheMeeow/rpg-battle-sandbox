cmake_minimum_required(VERSION 3.10)

project(RPGBattleSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 
set(HEADERS
    CoreTypes.h
    Weapons.h
    Actors.h
    GameLogic.h
)

set(SOURCES
    Weapons.cpp
    Actors.cpp
    GameLogic.cpp
    main.cpp
)

add_executable(GameApp ${HEADERS} ${SOURCES})