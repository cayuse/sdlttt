cmake_minimum_required(VERSION 3.3)
project(sdlttt)

# includes cmake/FindSDL2.cmake
# includes cmake/FindSDL2_image.cmake
# includes cmake/FindSDL2_ttf.cmake
set(CMAKE_MODULE_PATH ${sdlttt_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES
    src/bondiGame.cpp
    src/bondiGameInterface.h
    src/connect.cpp
    src/connect.h
    src/go.cpp
    src/go.h
    src/othello.cpp
    src/othello.h
    src/pente.cpp
    src/pente.h
    src/res_path.h
    src/ttt.cpp
    src/ttt.h)

add_executable(sdlttt ${SOURCE_FILES})
target_link_libraries(sdlttt ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})