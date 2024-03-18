project( notepad2020 )

# CMake 3.* necessario per Qt
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# programmiamo in C++ 11
set (CMAKE_CXX_STANDARD 11)

# ottengo tutti i sorgenti
file(GLOB my_sources *.h *.cpp *.qrc *.ui)

# attivo funzionalita' specifiche di CMake per Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# aggiungo eseguibile
add_executable(AutoPad ${my_sources})

# verifico l'esistenza delle librerie Qt necessarie
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# collego le librerie Qt con l'eseguibile
target_link_libraries(AutoPad Qt5::Widgets Qt5::PrintSupport)
