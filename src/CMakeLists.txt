cmake_minimum_required(VERSION 3.10)

# Libraries
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INDLUE_DIRS})

# Directories 
include_directories(../include)
set(ROMS_DIR ${CMAKE_HOME_DIRECTORY}/roms)
set(CHIP8_SOURCES main.c chip8.c display.c keyboard.c)

# Executables
add_executable(chip8 ${CHIP8_SOURCES})
target_link_libraries(chip8 ${SDL2_LIBRARIES})
add_executable(disassembler disassembler.c)

# make run
add_custom_target(run 
	COMMAND chip8 ${ROMS_DIR}/TestRom.ch8
	# COMMAND chip8 ${ROMS_DIR}/Breakout.ch8

	)

add_library(chip8_lib STATIC ${CHIP8_SOURCES})