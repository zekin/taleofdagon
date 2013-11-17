project(c009-taleofredengard)
find_package(OpenGL)
find_package(SDL)
find_package(SDL_mixer)
find_package(SDL_image)
add_executable(c009-taleofredengard main.cpp)
install(TARGETS c009-taleofredengard RUNTIME DESTINATION bin)

target_link_libraries(c009-taleofredengard ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARY} )