cmake_minimum_required(VERSION 3.20)

project(HexaView
	VERSION 1.0.0
	DESCRIPTION "binary file inspector written in c"
	LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSION OFF)

add_executable(HexaView
	src/main.c
	src/file_reader.c
	src/hex_dump.c
)

target_include_directories(HexaView PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(HexaView PRIVATE
	-Wall
	-Wextra
	-Wpedantic
	-Wconversion
	-Wshadow
)

include(CTest)

if(BUILD_TESTING)
	add_test(
		NAME HexVeiw_Integration
		COMMAND bash
			${CMAKE_CURRENT_SOURCE_DIR}/tests/test_reader.sh
			$<TARGET_FILE:HexaView>
		)
endif()
