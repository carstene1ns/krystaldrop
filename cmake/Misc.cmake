# This function will prevent in-source builds
function(AssureOutOfSourceBuilds)
	# make sure the user does not play dirty with symlinks
	get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
	get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

	# disallow in-source builds
	if("${srcdir}" STREQUAL "${bindir}")
		message("########################################################")
		message("# You must run cmake in a dedicated build directory.   #")
		message("#                                                      #")
		message("# 1) Run `git status` to find cmake's litter,          #")
		message("#    e.g. CMakeCache.txt and CMakeFiles directory      #")
		message("# 2) Remove it                                         #")
		message("# 3) Run `cmake -B build` to create a build directory. #")
		message("# 4) ...                                               #")
		message("# 5) PROFIT                                            #")
		message("#                                                      #")
		message("########################################################")
		message(FATAL_ERROR "In-source builds are disabled.")
	endif()
endfunction()
AssureOutOfSourceBuilds()

# ninja/parallel builds disable output colors, forcing them here.
# Needs to be at global scope to catch all targets.
# cmake has this build-in since 3.24
if(CMAKE_VERSION VERSION_LESS "3.24")
	option(CMAKE_COLOR_DIAGNOSTICS "Always produce ANSI-colored output" OFF)
	if(CMAKE_COLOR_DIAGNOSTICS)
		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			add_compile_options("-fdiagnostics-color=always")
		elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			add_compile_options("-fcolor-diagnostics")
		endif()
	endif()
endif()

# compiler cache
option(WANT_CCACHE "Use ccache to speed up rebuilds" OFF)
if(WANT_CCACHE)
	find_program(CCACHE_EXECUTABLE ccache)
	if(CCACHE_EXECUTABLE)
		set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
	endif()
endif()

# global scope, since ASAN needs to catch all targets
option(WANT_ASAN "build with address sanitizer" OFF)
if(WANT_ASAN)
	message(STATUS "Building with address sanitizer")
	add_compile_options(-fno-omit-frame-pointer -fsanitize=address)
	add_link_options(-fno-omit-frame-pointer -fsanitize=address)
endif()

# Endianess check
include(TestBigEndian)
test_big_endian(PLATFORM_BIGENDIAN)
if(PLATFORM_BIGENDIAN)
	add_compile_definitions(WORDS_BIGENDIAN=1)
endif()

if(NOT WIN32)
	string(ASCII 27 Esc)

	set(ColourReset "${Esc}[m")
	set(ColourBold  "${Esc}[1m")
	set(Red         "${Esc}[31m")
	set(Green       "${Esc}[32m")
	set(Yellow      "${Esc}[33m")
	set(Blue        "${Esc}[34m")
	set(Magenta     "${Esc}[35m")
	set(Cyan        "${Esc}[36m")
	set(White       "${Esc}[37m")
	set(BoldRed     "${Esc}[1;31m")
	set(BoldGreen   "${Esc}[1;32m")
	set(BoldYellow  "${Esc}[1;33m")
	set(BoldBlue    "${Esc}[1;34m")
	set(BoldMagenta "${Esc}[1;35m")
	set(BoldCyan    "${Esc}[1;36m")
	set(BoldWhite   "${Esc}[1;37m")
endif()

macro(print_color_summary)
	message(STATUS "${BoldBlue}-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-${ColourReset}")
	message(STATUS "${BoldYellow}${PROJECT_NAME}${ColourReset}"
		" Version ${BoldYellow}${PROJECT_VERSION}${ColourReset}")
	message(STATUS "")
	message(STATUS "Build type is set to ${BoldGreen}${CMAKE_BUILD_TYPE}${ColourReset}.")
	if(CCACHE_EXECUTABLE)
		message(STATUS "Using ${BoldCyan}ccache${ColourReset} as compiler launcher")
	endif()
	#message(STATUS "Using SDL2 ${BoldGreen}${SDL2_VERSION}${ColourReset}, "
	#	"mixer ${BoldGreen}${SDL2_mixer_VERSION}${ColourReset}, "
	#	"image ${BoldGreen}${SDL2_image_VERSION}${ColourReset} and "
	#	"ttf ${BoldGreen}${SDL2_ttf_VERSION}${ColourReset}.")
	message(STATUS "")
	message(STATUS "Homepage: ${BoldMagenta}${PROJECT_HOMEPAGE_URL}${ColourReset}")
	message(STATUS "Bug reports: ${BoldMagenta}${PROJECT_BUGREPORT}${ColourReset}")
	message(STATUS "${BoldBlue}-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-${ColourReset}")
endmacro()
