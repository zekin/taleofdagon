project(taleofdagon)
find_package(OpenGL)
find_package(SDL)
find_package(SDL_mixer)
find_package(SDL_image)
find_package(Log4cpp)
add_executable(taleofdagon main.cpp)
install(TARGETS taleofdagon RUNTIME DESTINATION bin)

target_link_libraries(taleofdagon ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARY} ${LOG4CPP_LIBRARY})

