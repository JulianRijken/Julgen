cmake_minimum_required(VERSION 3.26)
project(Julgen)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
  set(TARGET_NAME julgen_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME julgen)
  set(TARGET_PARAM WIN32)
endif()

option(USE_XINPUT "Switches between SDL and XInput" FALSE)

set(JULGEN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Julgen)


set(IMGUI_SOURCE_FILES
    #https://github.com/epezent/implot
    External/imgui/implot.cpp
    External/imgui/implot_demo.cpp
    External/imgui/implot_items.cpp
    External/imgui/implot_items.cpp

    External/imgui/backends/imgui_impl_sdl2
    External/imgui/backends/imgui_impl_sdlrenderer2

    External/imgui/imgui_demo.cpp

    External/imgui/imgui.cpp
    External/imgui/imgui_draw.cpp
    External/imgui/imgui_widgets.cpp
    External/imgui/imgui_tables.cpp
    External/imgui/backends/imgui_impl_sdl2.cpp
    External/imgui/backends/imgui_impl_sdlrenderer2.cpp
)


set(JULGEN_SOURCE_FILES
    Julgen/Components/SpriteRenderer.cpp
    Julgen/Components/TextRenderer.cpp
    Julgen/Components/Transform.cpp

    Julgen/Engine/GlobalSettings.h #HEADER
    Julgen/Engine/Singleton.h #HEADER

    Julgen/Engine/Animation.cpp
    Julgen/Engine/Command.cpp
    Julgen/Engine/Font.cpp
    Julgen/Engine/GameTime.cpp
    Julgen/Engine/GUI.cpp
    Julgen/Engine/Input.cpp
    Julgen/Engine/RenderManager.cpp
    Julgen/Engine/ResourceManager.cpp
    Julgen/Engine/Scene.cpp
    Julgen/Engine/SceneManager.cpp
    Julgen/Engine/Sprite.cpp
    Julgen/Engine/Texture2D.cpp

    Julgen/Game/AutoMove.cpp
    Julgen/Game/AutoRotateAround.cpp
    Julgen/Game/Bounce.cpp
    Julgen/Game/FpsCounter.cpp
    Julgen/Game/PlotExample.cpp

    Julgen/Main/Main.cpp
    Julgen/Main/Julgen.cpp

    Julgen/ObjectSystem/Component.cpp
    Julgen/ObjectSystem/GameObject.cpp
    Julgen/ObjectSystem/Object.cpp
    Julgen/ObjectSystem/Renderer.cpp
)



if(USE_XINPUT)
    message("InputImplXInput is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        Julgen/Engine/InputImplXInput.cpp
    )
else()
    message("InputImplSDL is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        Julgen/Engine/InputImplSDL.cpp
    )
endif()


# list source files here
add_executable(
    ${TARGET_NAME}
    ${TARGET_PARAM}
    ${IMGUI_SOURCE_FILES}
    ${JULGEN_SOURCE_FILES}
)

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ${JULGEN_SOURCE_DIR}
    ${JULGEN_SOURCE_DIR}/Components
    ${JULGEN_SOURCE_DIR}/Main
    ${JULGEN_SOURCE_DIR}/Engine
    ${JULGEN_SOURCE_DIR}/ObjectSystem
    ${JULGEN_SOURCE_DIR}/Game

    ${CMAKE_CURRENT_SOURCE_DIR}/External
    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/backends
)



if(USE_XINPUT)
    target_link_libraries(${TARGET_NAME} PUBLIC xinput)
endif()


# enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

set_property(TARGET ${TARGET_NAME} PROPERTY
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS FALSE
)


# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /MP>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# set julgen as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

include(FetchContent)


# add fmt
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.2.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(${TARGET_NAME} PUBLIC fmt::fmt)


# add glm
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)



# add SDL2
if (WIN32)
  FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.28.5-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads 
  )
  FetchContent_GetProperties(SDL2)
  if(NOT SDL2_POPULATED)
    FetchContent_Populate(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
  else()
    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
  endif()
  endif()

  # add SDL2_image
  FetchContent_Declare(
    SDL2_image
    URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.2-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_GetProperties(SDL2_image)
  if(NOT SDL2_image_POPULATED)
    FetchContent_Populate(SDL2_image)
    set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
  else()
    set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
  endif()
  endif()

  # add SDL2_ttf
  FetchContent_Declare(
    SDL2_ttf
    URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.2-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_GetProperties(SDL2_ttf)
  if(NOT sdl2_ttf_POPULATED)
    FetchContent_Populate(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
    set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
  else()
    set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
    set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
  endif()
  endif()

  set(VLD_DIR "$ENV{ProgramFiles\(x86\)}/Visual Leak Detector")
  IF(EXISTS ${VLD_DIR})
    set(VLD_INCLUDE_DIR "${VLD_DIR}/include")
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(VLD_LIBRARY "${VLD_DIR}/lib/Win64/vld.lib")
    else()
        set(VLD_LIBRARY "${VLD_DIR}/lib/Win32/vld.lib")
    endif()
  endif()

  # set all include directories
  target_include_directories(${TARGET_NAME} PRIVATE
    ${VLD_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_IMAGE_INCLUDE_DIR} 
    ${SDL2_TTF_INCLUDE_DIR})
  
  # set libraries to link with
  target_link_libraries(${TARGET_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES}
    ${VLD_LIBRARY}
  )
elseif(NOT EMSCRIPTEN)

  FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(SDL2)
  
  FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.2
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  set(SDL2IMAGE_INSTALL OFF)
  FetchContent_MakeAvailable(SDL2_image)

  FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.20.2
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  set(SDL2TTF_INSTALL OFF)
  FetchContent_MakeAvailable(SDL2_ttf)

  target_include_directories("${TARGET_NAME}" PUBLIC 
    ${sdl2_SOURCE_DIR}/include
    ${sdl2_image_SOURCE_DIR}/include
    ${sdl2_ttf_SOURCE_DIR}
  )
  target_link_libraries(${TARGET_NAME} PUBLIC 
    SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
  )
endif()


if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Content@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

  # work around https://github.com/emscripten-core/emscripten/issues/11513
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

  # Use the Emscripten toolchain
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

  # Additional settings for Emscripten build
  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  # Have emscripten generate a html page too.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  
elseif(WIN32)
  # fixes the working directory for the debugger, this will now find content in the correct directory
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  # add post build commands
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Content" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Content"
  )
endif()



