cmake_minimum_required(VERSION 3.3)
project(Prototype_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#On Ajoute les différentes sources au Projet
set(SOURCE_FILES    Sources/main.cpp
                    Header/Vecteur.h            Sources/Vecteur.cpp
                    Header/SceneSDL.h           Sources/SceneSDL.cpp
                    Header/Particule.h          Sources/Particule.cpp
                    Header/MatriceCreuse.h
                    Header/MatriceParticules.h  Sources/MatriceParticules.cpp
                    Header/Matiere.h            Sources/Matiere.cpp
                    Header/Grille.h             Sources/Grille.cpp
                    Header/Erreur.h             Sources/Erreur.cpp
                    Header/Definitions.h        Sources/Definitions.cpp)

add_executable(Prototype_1 ${SOURCE_FILES})

#Pour l'utilisation de la SDL :
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SDL2_INCLUDE_DIR C:/SDL2-2.0.4/include)
set(SDL2_LIBRARY C:/SDL2-2.0.4/lib/x64/SDL2.lib)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(Prototype_1 ${SDL2_LIBRARY})