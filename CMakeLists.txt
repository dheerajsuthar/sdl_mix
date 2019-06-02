set (CMAKE_CXX_STANDARD 14)
cmake_minimum_required (VERSION 3.10.2)	
project (sdl_mix)

set (sdl_mix_VERSION_MAJOR 1)
set (sdl_mix_VERSION_MINOR 0)
set(PKG_CONFIG_EXECUTABLE "/usr/bin/pkg-config")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_ttf SDL2_mixer SDL2_image SDL2_net )

include_directories (${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_net_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} 
${SDL2_image_INCLUDE_DIRS} ${SDL2_gfx_INCLUDE_DIRS})

configure_file (
	"${PROJECT_SOURCE_DIR}/sdl_mix_config.h.in"
	"${PROJECT_SOURCE_DIR}/include/sdl_mix_config.h"
	)

include_directories("${PROJECT_SOURCE_DIR}/include")
file(GLOB INC_ALL "${PROJECT_SOURCE_DIR}/include/*.h")

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

#add_executable (sdl_mix sdl_mix.cpp ${src})
add_executable (sdl_mix sdl_mix.cpp src/Game.cpp src/Sprite.cpp ${INC_ALL})
target_link_libraries (sdl_mix ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_net_LIBRARIES} ${SDL2_mixer_LIBRARIES} 
${SDL2_image_LIBRARIES} ${SDL2_gfx_LIBRARIES})
install (TARGETS sdl_mix DESTINATION bin)
install (FILES "${PROJECT_SOURCE_DIR}/include/sdl_mix_config.h"  DESTINATION include)
