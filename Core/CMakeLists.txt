project(CoreLib)

set(SOURCES
	src/Window.cpp
	src/Texture.cpp
	src/ECS.cpp
	src/EventHandler.cpp
)
add_subdirectory(vendor/SDL)
add_library(${PROJECT_NAME} ${SOURCES} "include/AppState.h") 
target_include_directories(${PROJECT_NAME} PUBLIC 
	include
	vendor/SDL/include
)

if(MINGW) 
	target_link_libraries(${PROJECT_NAME} mingw32)
endif()

target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)