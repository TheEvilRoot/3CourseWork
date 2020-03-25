cmake_minimum_required(VERSION 3.15)

set(CXX_STANDARD 14)

project(ANSIrC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_LOC /Development/Qt/5.12.6/clang_64)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "${QT_LOC}/lib/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/main.cpp src/socket.cpp src/socket.h src/buffer.cpp src/buffer.h src/client.cpp src/client.h src/irc.h)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)


