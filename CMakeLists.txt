project(taleofdagon)

if(json)
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

find_package(OpenGL)
find_package(SDL)
find_package(SDL_mixer)
find_package(SDL_image)

add_executable(taleofdagon src/main.cpp )
install(TARGETS taleofdagon RUNTIME DESTINATION bin)

target_link_libraries(taleofdagon ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARY})

include_directories(include)
include_directories(include/Interface)
include_directories(include/Enum)

add_subdirectory(src)
add_subdirectory(include/zekin)
