# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-src"
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-build"
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix"
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/tmp"
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
  "C:/Git/Julgen/cmake-build-debug/downloads"
  "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Git/Julgen/cmake-build-debug/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
