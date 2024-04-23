cmake_minimum_required(VERSION 3.26)
project(Julgen)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

set(TARGET_NAME julgen)


option(USE_XINPUT "Switches between SDL and XInput" FALSE)
option(USE_STEAMWORKS "Use steamworks SDK (must provide it in external/steamworks)" FALSE)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)


set(IMGUI_SOURCE_FILES
    #https://github.com/epezent/implot
    external/imgui/implot.cpp
    external/imgui/implot_demo.cpp
    external/imgui/implot_items.cpp
    external/imgui/implot_items.cpp

    external/imgui/backends/imgui_impl_sdl2
    external/imgui/backends/imgui_impl_sdlrenderer2

    external/imgui/imgui_demo.cpp

    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_sdlrenderer2.cpp
)


set(JULGEN_SOURCE_FILES
    ${SOURCE_DIR}/Engine/GameSettings.h
    ${SOURCE_DIR}/Engine/Singleton.h
    ${SOURCE_DIR}/Engine/Event.h
    ${SOURCE_DIR}/Engine/MathExtensions.h
    ${SOURCE_DIR}/Engine/Locator.h
    ${SOURCE_DIR}/Engine/Service.h

    ${SOURCE_DIR}/Engine/Physics.cpp                   ${SOURCE_DIR}/Engine/Physics.h
    ${SOURCE_DIR}/Engine/Command.cpp                   ${SOURCE_DIR}/Engine/Command.h
    ${SOURCE_DIR}/Engine/Font.cpp                      ${SOURCE_DIR}/Engine/Font.h
    ${SOURCE_DIR}/Engine/GameTime.cpp                  ${SOURCE_DIR}/Engine/GameTime.h
    ${SOURCE_DIR}/Engine/GUI.cpp                       ${SOURCE_DIR}/Engine/GUI.h
    ${SOURCE_DIR}/Engine/Input.cpp                     ${SOURCE_DIR}/Engine/Input.h
    ${SOURCE_DIR}/Engine/RenderManager.cpp             ${SOURCE_DIR}/Engine/RenderManager.h
    ${SOURCE_DIR}/Engine/ResourceManager.cpp           ${SOURCE_DIR}/Engine/ResourceManager.h
    ${SOURCE_DIR}/Engine/Scene.cpp                     ${SOURCE_DIR}/Engine/Scene.h
    ${SOURCE_DIR}/Engine/SceneManager.cpp              ${SOURCE_DIR}/Engine/SceneManager.h
    ${SOURCE_DIR}/Engine/Sprite.cpp                    ${SOURCE_DIR}/Engine/Sprite.h
    ${SOURCE_DIR}/Engine/Texture2D.cpp                 ${SOURCE_DIR}/Engine/Texture2D.h
    ${SOURCE_DIR}/Engine/Achievement.cpp               ${SOURCE_DIR}/Engine/Achievement.h
    ${SOURCE_DIR}/Engine/MessageQueue.cpp              ${SOURCE_DIR}/Engine/MessageQueue.h

    ${SOURCE_DIR}/Main/Main.cpp
    ${SOURCE_DIR}/Main/Julgen.cpp                      ${SOURCE_DIR}/Main/Julgen.h

    ${SOURCE_DIR}/ObjectSystem/Component.cpp           ${SOURCE_DIR}/ObjectSystem/Component.h
    ${SOURCE_DIR}/ObjectSystem/GameObject.cpp          ${SOURCE_DIR}/ObjectSystem/GameObject.h
    ${SOURCE_DIR}/ObjectSystem/Object.cpp              ${SOURCE_DIR}/ObjectSystem/Object.h
    ${SOURCE_DIR}/ObjectSystem/RenderComponent.cpp     ${SOURCE_DIR}/ObjectSystem/RenderComponent.h

    ${SOURCE_DIR}/Components/SpriteRenderer.cpp        ${SOURCE_DIR}/Components/SpriteRenderer.h
    ${SOURCE_DIR}/Components/TextRenderer.cpp          ${SOURCE_DIR}/Components/TextRenderer.h
    ${SOURCE_DIR}/Components/Transform.cpp             ${SOURCE_DIR}/Components/Transform.h
    ${SOURCE_DIR}/Components/Animator.cpp              ${SOURCE_DIR}/Components/Animator.h
    ${SOURCE_DIR}/Components/Rigidbody.cpp             ${SOURCE_DIR}/Components/Rigidbody.h

)



if(USE_XINPUT)
    message("InputImplXInput is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        ${SOURCE_DIR}/Engine/InputImplXInput.cpp
    )
else()
    message("InputImplSDL is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        ${SOURCE_DIR}/Engine/InputImplSDL.cpp
    )
endif()


add_library(${TARGET_NAME} STATIC
    ${IMGUI_SOURCE_FILES}
    ${JULGEN_SOURCE_FILES}
)


target_include_directories(${TARGET_NAME} PUBLIC
    ${SOURCE_DIR}/Components
    ${SOURCE_DIR}/Main
    ${SOURCE_DIR}/Engine
    ${SOURCE_DIR}/ObjectSystem

    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
)



if(USE_XINPUT)
    target_link_libraries(${TARGET_NAME} PUBLIC xinput)
endif()

# Credits to All-Purpose Mat https://allpurposem.at/ for providing steamworks cmake inegration
if(USE_STEAMWORKS)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/steamworks")
        message(FATAL_ERROR "Steamworks directory not found. Please place the contents of the Steamworks zip file at external/steamworks")
    else()
        file(GLOB STEAMWORKS_FILES "${CMAKE_SOURCE_DIR}/external/steamworks/*")
        if(NOT STEAMWORKS_FILES)
            message(FATAL_ERROR "Steamworks directory is empty. Please make sure it contains the necessary files.")
        endif()
    endif()


    set(STEAMWORKS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/steamworks/public/steam)
    set(STEAMWORKS_LIB_DIR ${CMAKE_SOURCE_DIR}/external/steamworks/redistributable_bin)

    if(WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/win64/steam_api64.lib)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${STEAMWORKS_LIB_DIR}/win64/steam_api64.dll
                           $<TARGET_FILE_DIR:${TARGET_NAME}>)

        else()
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/steam_api.lib)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${STEAMWORKS_LIB_DIR}/steam_api.dll
                           $<TARGET_FILE_DIR:${TARGET_NAME}>)
        endif()

    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/linux64/libsteam_api.so)
        else()
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/linux32/libsteam_api.so)
        endif()
    endif()

    target_include_directories(${TARGET_NAME} PRIVATE ${STEAMWORKS_INCLUDE_DIR})
    target_link_libraries(${TARGET_NAME} PRIVATE ${STEAMWORKS_LIB})

    target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo 480 > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
        COMMENT "Creating steam_appid.txt"
    )
endif()


function(HandleAssets GAME_TARGET_DIR)
    if(WIN32)
        set(SYMLINK_COMMAND copy_directory)
    else()
        set(SYMLINK_COMMAND create_symlink)
    endif()

    add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E ${SYMLINK_COMMAND}
                    "${CMAKE_SOURCE_DIR}/Assets"
                    "${GAME_TARGET_DIR}/Assets")
endfunction()

# enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
set_property(TARGET ${TARGET_NAME} PROPERTY
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS FALSE
)

# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /MP>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)


# add box2D
add_subdirectory(external/Box2D-cmake)
target_link_libraries(${TARGET_NAME} PUBLIC tobanteGaming::Box2D)


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
  target_include_directories(${TARGET_NAME} PUBLIC
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
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Assets@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

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
  # fixes the working directory for the debugger, this will now find assets in the correct directory
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
endif()



