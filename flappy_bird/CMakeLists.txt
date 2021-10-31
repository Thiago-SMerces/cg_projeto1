project(flappy_bird)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp pipes.cpp bird.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})