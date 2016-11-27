project(SpheroController)

add_definitions(-std=c++11)

set(SRC_DIR src)
set(LIB_DIR libs)
set(LIB_SDL_DIR ${LIB_DIR}/SDL2-2.0.5)

include_directories(
    ${SRC_DIR}
    ${LIB_SDL_DIR}/include
    )

link_directories(
    ${LIB_SDL_DIR}/build
    )


set(SOURCES 
    ${SRC_DIR}/controller.hpp
    ${SRC_DIR}/controller.cpp
    )

add_executable(SpheroController ${SRC_DIR}/main.cpp ${SOURCES})
#[[target_link_libraries(SpheroController PRIVATE
  "${LIB_SDL_DIR}/build/libSDL2main.a"
  "${LIB_SDL_DIR}/build/libSDL2.a"
  )]]