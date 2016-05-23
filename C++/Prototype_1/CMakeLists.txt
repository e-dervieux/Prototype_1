cmake_minimum_required(VERSION 3.3)
project(Prototype_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32 -lSDL2main -lSDL2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lshell32 -lversion -luuid")

set(SOURCE_FILES    Sources/main.cpp
        Header/Erreur.h             Sources/Erreur.cpp
        Header/Vecteur.h            Sources/Vecteur.cpp
        Header/Element.h
        Header/Conteneur.h          Sources/Conteneur.cpp
        Header/MatriceCreuse.h
        Header/MatriceParticules.h
        Header/Particule.h          Sources/Particule.cpp
        Header/Matiere.h            Sources/Matiere.cpp
        Header/Definitions.h        Sources/Definitions.cpp
        Header/SceneSDL.h           Sources/SceneSDL.cpp
        Header/demoMatriceParticules.h
        Header/demoLiaison.h
        Header/demoCohesion.h
        Header/demoCollision1.h
        Header/demoLiaisonsMatrices.h
        Header/demoCollisionMatrices.h
        Header/demoCollision2.h
        Header/LiaisonsMC.h         Sources/LiaisonsMC.cpp
        Header/Brisure.h Header/SolideImage.h Sources/SolideImage.cpp Header/demoImage.h)

add_executable(Prototype_1 ${SOURCE_FILES})
