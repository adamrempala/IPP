cmake_minimum_required(VERSION 3.0)
project(Drogi C)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

# Nie chcemy oglÄ…daÄ‡ komunikatA3w o procentowym postÄ™pie kompilacji.
set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)

# Chcemy zobaczyÄ‡ polecenia wywoA‚ane przez make.
set(CMAKE_VERBOSE_MAKEFILE ON)

# Ustawiamy wspA3lne opcje kompilowania dla wszystkich wariantA3w projektu.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra")
# DomyA›lne opcje dla wariantA3w Release i Debug sÄ… sensowne.
# JeA›li to konieczne, ustawiamy tu inne.
# set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
# set(CMAKE_C_FLAGS_DEBUG "-g")

# Wskazujemy pliki AorA3dA‚owe.
set(SOURCE_FILES
    src/strdup.c
    src/strdup.h
    src/prime.c
    src/prime.h
    src/hashtable.c
    src/hashtable.h
    src/road.c
    src/road.h
    src/priqueue.c
    src/priqueue.h
    src/dijkstra.c
    src/dijkstra.h
    src/map.c
    src/map.h
    src/map_main.c)

# Wskazujemy plik wykonywalny.
add_executable(map ${SOURCE_FILES})

# Wskazujemy biblioteki.
target_link_libraries(map m)

# Dodajemy obsA‚ugÄ™ Doxygena: sprawdzamy, czy jest zainstalowany i jeA›li tak to:
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Wskazujemy lokalizacjÄ… pliku konfiguracyjnego i podajemy jego docelowÄ… lokalizacjÄ™ w folderze, gdzie nastÄ™puje kompilacja.
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    # Dodajemy cel doc: uA1ycie make doc spowoduje wywoA‚anie doxygena z odpowiednim plikiem konfiguracyjnym w folderze kompilacji.
    # Na wyjA›cie zostanie wypisany odpowiedni komentarz.
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif (DOXYGEN_FOUND)
