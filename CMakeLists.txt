cmake_minimum_required(VERSION 2.8)
project(TaleOfDagon)

#libraries
##OpenGL
find_package(OpenGL REQUIRED)

link_directories("${OPENGL_LIBRARY}")

##SDL
find_package(SDL REQUIRED)

include_directories("${SDL_INCLUDE_DIR}")
link_directories("${SDL_LIBRARY}")

###include winmain?

##SDL_mixer
find_package(SDL_mixer REQUIRED)

include_directories("${SDLMIXER_INCLUDE_DIR}")
link_directories("${SDLMIXER_LIBRARY}")

##SDL_image
find_package(SDL_image REQUIRED)

include_directories("${SDLIMAGE_INCLUDE_DIR}")
link_directories("${SDLIMAGE_LIBRARY}")

#add project headers
include_directories("${PROJECT_SOURCE_DIR}/include")

#everything else is in src/
add_subdirectory(src)
