cmake_minimum_required(VERSION 3.19...4.2)

project(KrystalDrop VERSION 0.8.0 LANGUAGES CXX
  DESCRIPTION "Remake of an old Neo Geo game called Magical Drop (1996)"
  HOMEPAGE_URL "https://github.com/carstene1ns/krystaldrop")
set(PROJECT_BUGREPORT "https://github.com/carstene1ns/krystaldrop/issues")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(Misc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set build type to debug, if not set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
else()
  set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel.")
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# external libs
#find_package(SDL2 REQUIRED)
#find_package(SDL2_mixer REQUIRED)
#find_package(SDL2_image REQUIRED)
#find_package(SDL2_ttf REQUIRED)
find_package(ZLIB REQUIRED)
find_package(libxml2 REQUIRED)

# TODO
add_compile_definitions(CFGDIR="/etc" BINDIR="/usr/share/krystaldrop")
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
add_compile_options(-Wall)
#add_compile_options(-Wextra)

# minimal
#add_compile_definitions(NO_OPENGL NO_MUSIC NO_SOUND NO_MIXER)

# program + library
add_subdirectory(Sources)

# summary
print_color_summary()
