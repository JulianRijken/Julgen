cmake_minimum_required(VERSION 3.26)
project(Julgen)

# Options
option(USE_XINPUT "Switches between SDL and XInput" FALSE)
option(USE_STEAMWORKS "Use steamworks SDK (must provide it in external/steamworks)" FALSE)
option(USE_VLD "Use VLD on windows" TRUE)

# Sets
set(TARGET_NAME julgen)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Append cmake libraries file
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Libraries)

# Info
if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Use Libraries file and fetch / find all needed libs
make_libs_available()

if(WIN32)
    if(USE_VLD)
    make_vld_available()
endif()
endif()


set(IMGUI_SOURCE_FILES
    #https://github.com/epezent/implot
    ${EXTERNAL_DIR}/imgui/implot.cpp
    ${EXTERNAL_DIR}/imgui/implot_demo.cpp
    ${EXTERNAL_DIR}/imgui/implot_items.cpp
    ${EXTERNAL_DIR}/imgui/implot_items.cpp

    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_sdl2
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_sdlrenderer2

    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp

    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_sdl2.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_sdlrenderer2.cpp
)


set(JULGEN_SOURCE_FILES
    ${SOURCE_DIR}/Engine/GameSettings.h
    ${SOURCE_DIR}/Engine/Singleton.h
    ${SOURCE_DIR}/Engine/Event.h
    ${SOURCE_DIR}/Engine/MathExtensions.h
    ${SOURCE_DIR}/Engine/Locator.h
    ${SOURCE_DIR}/Engine/Service.h

    ${SOURCE_DIR}/Engine/Sound.h                        ${SOURCE_DIR}/Engine/Sound.cpp
    ${SOURCE_DIR}/Engine/Physics.cpp                    ${SOURCE_DIR}/Engine/Physics.h
    ${SOURCE_DIR}/Engine/Command.cpp                    ${SOURCE_DIR}/Engine/Command.h
    ${SOURCE_DIR}/Engine/Font.cpp                       ${SOURCE_DIR}/Engine/Font.h
    ${SOURCE_DIR}/Engine/GameTime.cpp                   ${SOURCE_DIR}/Engine/GameTime.h
    ${SOURCE_DIR}/Engine/GUI.cpp                        ${SOURCE_DIR}/Engine/GUI.h
    ${SOURCE_DIR}/Engine/Input.cpp                      ${SOURCE_DIR}/Engine/Input.h
    ${SOURCE_DIR}/Engine/RenderManager.cpp              ${SOURCE_DIR}/Engine/RenderManager.h
    ${SOURCE_DIR}/Engine/ResourceManager.cpp            ${SOURCE_DIR}/Engine/ResourceManager.h
    ${SOURCE_DIR}/Engine/Scene.cpp                      ${SOURCE_DIR}/Engine/Scene.h
    ${SOURCE_DIR}/Engine/SceneManager.cpp               ${SOURCE_DIR}/Engine/SceneManager.h
    ${SOURCE_DIR}/Engine/Sprite.cpp                     ${SOURCE_DIR}/Engine/Sprite.h
    ${SOURCE_DIR}/Engine/Texture2D.cpp                  ${SOURCE_DIR}/Engine/Texture2D.h
    ${SOURCE_DIR}/Engine/Achievement.cpp                ${SOURCE_DIR}/Engine/Achievement.h
    ${SOURCE_DIR}/Engine/MessageQueue.cpp               ${SOURCE_DIR}/Engine/MessageQueue.h

    ${SOURCE_DIR}/Main/Main.cpp
    ${SOURCE_DIR}/Main/Julgen.cpp                       ${SOURCE_DIR}/Main/Julgen.h

    ${SOURCE_DIR}/ObjectSystem/Component.cpp            ${SOURCE_DIR}/ObjectSystem/Component.h
    ${SOURCE_DIR}/ObjectSystem/GameObject.cpp           ${SOURCE_DIR}/ObjectSystem/GameObject.h
    ${SOURCE_DIR}/ObjectSystem/Object.cpp               ${SOURCE_DIR}/ObjectSystem/Object.h
    ${SOURCE_DIR}/ObjectSystem/RenderComponent.cpp      ${SOURCE_DIR}/ObjectSystem/RenderComponent.h

    ${SOURCE_DIR}/Components/SpriteRenderer.cpp         ${SOURCE_DIR}/Components/SpriteRenderer.h
    ${SOURCE_DIR}/Components/TextRenderer.cpp           ${SOURCE_DIR}/Components/TextRenderer.h
    ${SOURCE_DIR}/Components/Transform.cpp              ${SOURCE_DIR}/Components/Transform.h
    ${SOURCE_DIR}/Components/Animator.cpp               ${SOURCE_DIR}/Components/Animator.h
    ${SOURCE_DIR}/Components/Rigidbody.cpp              ${SOURCE_DIR}/Components/Rigidbody.h
    ${SOURCE_DIR}/Components/BoxCollider.cpp            ${SOURCE_DIR}/Components/BoxCollider.h
)

# Switch between sld implementation and xinput implementation
if(USE_XINPUT)
    message("InputImplXInput is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        ${SOURCE_DIR}/Engine/InputImplXInput.cpp)
else()
    message("InputImplSDL is enabled")
    list(APPEND JULGEN_SOURCE_FILES
        ${SOURCE_DIR}/Engine/InputImplSDL.cpp)
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

    ${SOLOUD_INCLUDE_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
)

# Link XInput
if(USE_XINPUT)
    target_link_libraries(${TARGET_NAME} PUBLIC xinput)
endif()


# Add and Link box2D
# TODO: Set to public, idealy private, the game does not need to know about box2D
add_subdirectory(external/Box2D-cmake)
target_link_libraries(${TARGET_NAME} PUBLIC tobanteGaming::Box2D)


# Link SDL
if(MSVC)
    target_link_libraries(${TARGET_NAME} PUBLIC SDL2::SDL2main)
endif()
target_link_libraries(${TARGET_NAME} PUBLIC
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)

target_link_libraries(${TARGET_NAME} PUBLIC
    glm::glm
)

target_link_libraries(${TARGET_NAME} PUBLIC
    fmt::fmt
)

target_link_libraries(${TARGET_NAME} PRIVATE
    soloud
)




if(USE_STEAMWORKS)
    use_steamworks(${TARGET_NAME})
endif()

if(USE_VLD)
    if (WIN32)
        target_include_directories(${TARGET_NAME} PUBLIC ${VLD_INCLUDE_DIR})
        target_link_libraries(${TARGET_NAME} PUBLIC ${VLD_LIBRARY})
    endif()
endif()


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

# Make sure the asset folder get's copyed or linked correctly
if(WIN32)
    set(SYMLINK_COMMAND copy_directory)
else()
    set(SYMLINK_COMMAND create_symlink)
endif()

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E ${SYMLINK_COMMAND}
                "${CMAKE_SOURCE_DIR}/Assets"
                "${CMAKE_BINARY_DIR}/Assets")
