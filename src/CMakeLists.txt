cmake_minimum_required(VERSION 3.10)

include_directories(../include)
set(ROMS_DIR ${CMAKE_HOME_DIRECTORY}/roms)
set(CHIP8_SOURCES main.c chip8.c)

add_executable(chip8 ${CHIP8_SOURCES})
add_executable(disassembler disassembler.c)

add_custom_target(run 
	COMMAND chip8 ${ROMS_DIR}/Tetris\ [Fran\ Dachille,\ 1991].ch8
	)