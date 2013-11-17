add_executable(taleofdagon
                main.cpp)
target_link_libraries(taleofdagon ${SDL_LIBRARIES} ${SDLMIXER_LIBRARIES} ${SDLIMAGE_LIBRARIES} ${OPENGL_LIBRARIES})
