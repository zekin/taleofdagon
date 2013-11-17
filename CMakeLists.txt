cmake_minimum_required(VERSION 2.8)
project(TaleOfDagon)

#libraries
##OpenGL
find_package(OpenGL REQUIRED)

#link_directories("${OPENGL_LIBRARIES}")

##SDL
find_package(SDL REQUIRED)

include_directories("${SDL_INCLUDE_DIR}")
link_directories("${SDL_LIBRARIES}")

###include winmain?

##SDL_mixer
find_package(SDL_mixer REQUIRED)

include_directories("${SDL_MIXER_INCLUDE_DIRS}")
link_directories("${SDL_MIXER_LIBRARIES}")

##SDL_image
find_package(SDL_image REQUIRED)

include_directories("${SDL_IMAGE_INCLUDE_DIRS}")
link_directories("${SDL_IMAGE_LIBRARIES}")

#add project headers
include_directories("${PROJECT_SOURCE_DIR}/include")

#everything else is in src/
add_subdirectory(src)
