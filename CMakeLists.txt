cmake_minimum_required(VERSION 3.20.0)

project(
	ParticleSwarm
)

add_library(
	${PROJECT_NAME}
)

SET(PARTICLE_SWARM_NUM_DIMENSIONS 1 CACHE STRING "Number of dimensions for the Particle Swarm Optimization algorithm")
SET(PARTICLE_SWARM_NUM_THREADS 1 CACHE STRING "Number of optimization threads to use")

# Doxygen document generation
find_package(Doxygen)
if (DOXYGEN_FOUND AND NOT DOXYGEN_IN)

	# Set input configuration
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

	# Add documentation target
	add_custom_target(
		docs
		COMMAND
			${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
		WORKING_DIRECTORY
			${CMAKE_CURRENT_BINARY_DIR}
		COMMENT
			"Generate documentation with Doxygen"
		VERBATIM
	)
endif()

target_compile_options(
	${PROJECT_NAME}
	PRIVATE
		"-Wall" # Enable all common warnings
		"-Wextra" # Enable more warnings
		"-Wpedantic" # Warn about non-standard code
		"-Werror" # Warnings are treated as errors
		"-Wshadow" # Warn about shadow variables
		"-Wdouble-promotion" # Warn when promoting floats to doubles
		"-Wformat=2" # printf and scanf warnings
		"-Wformat-security" # printf and scanf warnings
		"-Wformat-truncation" # printf and scanf warnings
		"-Wundef" # Warn when evaluating not defined flags
		"-fno-common" # Warn about multiple global variables with the same name
		"-Wuninitialized" # Warn about uninitialized’ variables
		#"-Wconversion" # Warn about implicit conversions
		#"-Wpadded" # Warn about padded structures
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
		"-ffunction-sections" # Link time optimization help
		"-fdata-sections" # Link time optimization help
		"-fanalyzer" # Static analysis of program flow
)

target_link_options(
	${PROJECT_NAME}
	PRIVATE
		"-Wl,--gc-sections" # Remove unwanted/unused sections
		"-flto" # Link time optimization
		"-fno-fat-lto-objects" # Link time optimization
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		include
)

target_compile_definitions(
	${PROJECT_NAME}
	PUBLIC
		"SWARM_DIMENSIONS=${PARTICLE_SWARM_NUM_DIMENSIONS}"
	PRIVATE
		"SWARM_THREADS=${PARTICLE_SWARM_NUM_THREADS}"
)

if (PARTICLE_SWARM_NUM_THREADS GREATER 1)
	find_package(Threads REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c")

target_sources(
	${PROJECT_NAME}
	PUBLIC
		${INCLUDE_FILES}
	PRIVATE
		${SOURCE_FILES}
)
