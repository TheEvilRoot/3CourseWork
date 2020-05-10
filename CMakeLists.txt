cmake_minimum_required(VERSION 3.15)

set(CXX_STANDARD 17)

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

link_directories(lib)
include_directories(include)

set(INCLUDE_FILES
        include/client.h
        include/irc.h
        include/socket.h
        include/buffer.h
        include/handler.h
        include/message.h
        include/parser.h
        include/listener.h
        )

set(LIB_FILES
        lib/client.cpp
        lib/socket.cpp
        lib/buffer.cpp
        lib/handler.cpp
        lib/parser.cpp
        )

set(SOURCE_FILES
        src/main.cpp)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
        ${LIB_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)


