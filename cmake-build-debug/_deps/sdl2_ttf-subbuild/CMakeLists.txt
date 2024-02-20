# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.27.8)

# Reject any attempt to use a toolchain file. We must not use one because
# we could be downloading it here. If the CMAKE_TOOLCHAIN_FILE environment
# variable is set, the cache variable will have been initialized from it.
unset(CMAKE_TOOLCHAIN_FILE CACHE)
unset(ENV{CMAKE_TOOLCHAIN_FILE})

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(sdl2_ttf-populate NONE)



include(ExternalProject)
ExternalProject_Add(sdl2_ttf-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.2-VC.zip" "DOWNLOAD_NO_PROGRESS" "ON" "DOWNLOAD_DIR" "C:/Git/Julgen/cmake-build-debug/downloads"
                    SOURCE_DIR          "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_ttf-src"
                    BINARY_DIR          "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_ttf-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


