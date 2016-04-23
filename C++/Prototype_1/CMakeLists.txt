cmake_minimum_required(VERSION 3.3)
project(Prototype_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32 -lSDL2main -lSDL2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lshell32 -lversion -luuid -static-libgcc")

#On Ajoute les différentes sources au Projet

#Cette commande peut être remplacée par :
#file(GLOB_RECURSE SOURCES Sources/*)
#file(GLOB_RECURSE HEADERS Header/*), les headers sont-ils vraiment nécessaires ?
#set(SOURCE_FILES ${SOURCES} ${HEADERS})

set(SOURCE_FILES    Sources/main.cpp
                    Header/Vecteur.h            Sources/Vecteur.cpp
                    Header/SceneSDL.h           Sources/SceneSDL.cpp
                    Header/Element.h
                    Header/Conteneur.h
                    Header/Particule.h          Sources/Particule.cpp
                    Header/MatriceCreuseDemo.h
        Header/MatriceParticulesOld.h Sources/MatriceParticulesOld.cpp
                    Header/Matiere.h            Sources/Matiere.cpp
                    Header/Erreur.h             Sources/Erreur.cpp
                    Header/Definitions.h        Sources/Definitions.cpp
                    Header/demoMatriceCreuse.h
                    Header/demoMatriceParticules.h
                    Header/demoGraphique1.h
                    Header/demoLiaison.h
                    Header/demoCohesion.h
                    Header/demoCollision1.h
                    Header/MatriceCreuse.h
        Header/MatriceParticules.h
                    Header/Conteneur.h Header/demoMatricesRecursives.h Header/demoLiaisonsMatrices.h Header/demoCollisionMatrices.h)

add_executable(Prototype_1 ${SOURCE_FILES})

#Pour l'utilisation de la SDL :
