cmake_minimum_required(VERSION 3.18)
project(Julgen)

# Options
option(USE_STEAMWORKS "Use steamworks SDK (must provide it in external/steamworks)" FALSE)
option(USE_VLD "Use VLD on windows" TRUE)
option(USE_ADDRESS_SANITIZER "Use VLD on windows" FALSE)


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
    ${SOURCE_DIR}/Main/Main.cpp # CPP
    ${SOURCE_DIR}/Main/Julgen.h                         ${SOURCE_DIR}/Main/Julgen.cpp

    # Input
    ${SOURCE_DIR}/Engine/Input/InputContext.h
    ${SOURCE_DIR}/Engine/Input/InputBinding.h
    ${SOURCE_DIR}/Engine/Input/InputAction.h
    ${SOURCE_DIR}/Engine/Input/Input.h                  ${SOURCE_DIR}/Engine/Input/Input.cpp

    # Physics
    ${SOURCE_DIR}/Engine/Physics/Physics.h              ${SOURCE_DIR}/Engine/Physics/Physics.cpp
    ${SOURCE_DIR}/Engine/Physics/ICollisionListener.h

    # Resources
    ${SOURCE_DIR}/Engine/Resources/Resources.h    ${SOURCE_DIR}/Engine/Resources/Resources.cpp
    ${SOURCE_DIR}/Engine/Resources/Texture2D.h          ${SOURCE_DIR}/Engine/Resources/Texture2D.cpp
    ${SOURCE_DIR}/Engine/Resources/Sprite.h             ${SOURCE_DIR}/Engine/Resources/Sprite.cpp
    ${SOURCE_DIR}/Engine/Resources/Font.h               ${SOURCE_DIR}/Engine/Resources/Font.cpp

    # Sound
    ${SOURCE_DIR}/Engine/Sound/Sound.h
    ${SOURCE_DIR}/Engine/Sound/Sound_Null.h
    ${SOURCE_DIR}/Engine/Sound/Sound_Logging.h
    ${SOURCE_DIR}/Engine/Sound/Sound_System.h           ${SOURCE_DIR}/Engine/Sound/Sound_System_SoLoud_Impl.cpp

    ${SOURCE_DIR}/Engine/GameSettings.h
    ${SOURCE_DIR}/Engine/MathExtensions.h
    ${SOURCE_DIR}/Engine/GameTime.h                     ${SOURCE_DIR}/Engine/GameTime.cpp
    ${SOURCE_DIR}/Engine/EngineGUI.h                    ${SOURCE_DIR}/Engine/EngineGUI.cpp
    ${SOURCE_DIR}/Engine/Achievement.h                  ${SOURCE_DIR}/Engine/Achievement.cpp
    ${SOURCE_DIR}/Engine/Scene.h                        ${SOURCE_DIR}/Engine/Scene.cpp
    ${SOURCE_DIR}/Engine/RenderManager.h                ${SOURCE_DIR}/Engine/RenderManager.cpp
    ${SOURCE_DIR}/Engine/SceneManager.h                 ${SOURCE_DIR}/Engine/SceneManager.cpp

    # Tweening
    ${SOURCE_DIR}/Engine/Tweening/Tween.h               ${SOURCE_DIR}/Engine/Tweening/Tween.cpp
    ${SOURCE_DIR}/Engine/Tweening/TweenEngine.h         ${SOURCE_DIR}/Engine/Tweening/TweenEngine.cpp
    ${SOURCE_DIR}/Engine/Tweening/TweenInstance.h       ${SOURCE_DIR}/Engine/Tweening/TweenInstance.cpp
    ${SOURCE_DIR}/Engine/Tweening/EaseFunction.h

    # Acces
    ${SOURCE_DIR}/Engine/Singleton.h
    ${SOURCE_DIR}/Engine/Locator.h
    ${SOURCE_DIR}/Engine/Service.h

    # Communication
    ${SOURCE_DIR}/Engine/Event.h
    ${SOURCE_DIR}/Engine/Command.h                      ${SOURCE_DIR}/Engine/Command.cpp
    ${SOURCE_DIR}/Engine/MessageQueue.h                 ${SOURCE_DIR}/Engine/MessageQueue.cpp

    # Object System
    ${SOURCE_DIR}/ObjectSystem/Component.h              ${SOURCE_DIR}/ObjectSystem/Component.cpp
    ${SOURCE_DIR}/ObjectSystem/GameObject.h             ${SOURCE_DIR}/ObjectSystem/GameObject.cpp
    ${SOURCE_DIR}/ObjectSystem/Object.h                 ${SOURCE_DIR}/ObjectSystem/Object.cpp
    ${SOURCE_DIR}/ObjectSystem/RenderComponent.h        ${SOURCE_DIR}/ObjectSystem/RenderComponent.cpp
    ${SOURCE_DIR}/ObjectSystem/Transform.h              ${SOURCE_DIR}/ObjectSystem/Transform.cpp

    # Components
    ${SOURCE_DIR}/Components/SpriteRenderer.h           ${SOURCE_DIR}/Components/SpriteRenderer.cpp
    ${SOURCE_DIR}/Components/TextRenderer.h             ${SOURCE_DIR}/Components/TextRenderer.cpp
    ${SOURCE_DIR}/Components/Animator.h                 ${SOURCE_DIR}/Components/Animator.cpp
    ${SOURCE_DIR}/Components/Rigidbody.h                ${SOURCE_DIR}/Components/Rigidbody.cpp
    ${SOURCE_DIR}/Components/BoxCollider.h              ${SOURCE_DIR}/Components/BoxCollider.cpp
    ${SOURCE_DIR}/Components/Camera.h                   ${SOURCE_DIR}/Components/Camera.cpp
)


add_library(${TARGET_NAME} STATIC
    ${IMGUI_SOURCE_FILES}
    ${JULGEN_SOURCE_FILES}
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${SOURCE_DIR}/Components
    ${SOURCE_DIR}/Main
    ${SOURCE_DIR}/Engine
    ${SOURCE_DIR}/Engine/Input
    ${SOURCE_DIR}/Engine/Physics
    ${SOURCE_DIR}/Engine/Resources
    ${SOURCE_DIR}/Engine/Sound
    ${SOURCE_DIR}/Engine/Tweening
    ${SOURCE_DIR}/ObjectSystem

    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
)




if(MSVC)
    target_link_libraries(${TARGET_NAME} PUBLIC SDL2::SDL2main)
endif()


target_link_libraries(${TARGET_NAME} PUBLIC
    glm::glm
    fmt::fmt
    tobanteGaming::Box2D #TODO:  Needs to also be PRIVATE once abstraction finished
)


target_link_libraries(${TARGET_NAME} PRIVATE
    soloud
)


if(EMSCRIPTEN)
    target_compile_options(${TARGET_NAME} PUBLIC -pthread -gsource-map -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -Wno-switch)
    target_link_libraries(${TARGET_NAME} PUBLIC -sERROR_ON_UNDEFINED_SYMBOLS=0 -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2)

    SET(CMAKE_EXECUTABLE_SUFFIX ".html")

    target_link_libraries(${TARGET_NAME} PUBLIC
        "--preload-file ${CMAKE_SOURCE_DIR}/Assets@Assets"
        "-s ALLOW_MEMORY_GROWTH" 
        "-s EXIT_RUNTIME=1"
        "-s SDL2_IMAGE_FORMATS='[\"png\"]'"
        "-s ASSERTIONS"
        "-s GL_DEBUG"
        "-s DEMANGLE_SUPPORT=1"
        "-s OFFSCREEN_FRAMEBUFFER=1"

        "--shell-file \"${EMSCRIPTEN_ROOT_PATH}/src/shell_minimal.html\""
    )





    # # modify shell_minimal.html to make canvas fill the screen
    # file(READ ${EMSCRIPTEN_ROOT_PATH}/src/shell_minimal.html EMSCRIPTEN_SHELL_HTML)
    # string(FIND "${EMSCRIPTEN_SHELL_HTML}" "<style>" HAS_STYLE)
    # if (${HAS_STYLE} EQUAL -1)
    #     message(WARNING "<style> not found in shell_minimal.html, copying as-is")
    # else()
    #     string(CONCAT STYLE_REPLACEMENT "<meta name=\"viewport\" content=\"width=device-width,user-scalable=no,viewport-fit=cover\">\n"
    #         "    <meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n"
    #         "    <style>\n"
    #         "      body, html { border: 0px none; padding: 0px; margin: 0px; width: 100%; height: 100%; overflow: hidden; position: fixed; }\n"
    #         "      canvas.emscripten {"
    #         "          image-rendering: crisp-edges;"
    #         "          image-rendering: pixelated;"
    #         "          aspect-ratio: ${ASPECT_RATIO};"
    #         "          max-height: 100vh;"
    #         "          max-width: 100vw;"
    #         "          width: min(100vw, calc(100vh * calc(${ASPECT_RATIO})));"
    #         "          height: min(100vh, calc(100vw / calc(${ASPECT_RATIO})));"
    #         "          overflow: hidden;"
    #         "          display: block;"
    #         "          position: absolute;"
    #         "          top: 0; bottom: 0; left: 0; right: 0;"
    #         "          margin: auto;"
    #         "      }\n"
    #         "      .emscripten_border { width: 100%; height: 100%; border: 0px none !important;}\n"
    #         "      hr { display: none; }\n"
    #         "      div[id=\"overlay\"] {"
    #         "          position: absolute;"
    #         "          top: 0; left: 0;"
    #         "          width: 100%; height: 100%;"
    #         "          background-color: rgba(0, 0, 0, 0.5);"
    #         "          z-index: 9999;"
    #         "          display: flex;"
    #         "          justify-content: center;"
    #         "          align-items: center;"
    #         "          cursor: pointer;"
    #         "          text-shadow: 0 0 5px rgba(0, 0, 0, 0.8);"
    #         "     }\n"
    #         "     div[id=\"overlay\"] p {"
    #         "          background-color: #1E1E2E;"
    #         "          color: white;"
    #         "          font-size: 24px;"
    #         "          padding: 10px;"
    #         "          border-radius: 4px;"
    #         "          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);"
    #         "     }\n"
    #     )
    #     string(REPLACE "<style>" "${STYLE_REPLACEMENT}" EMSCRIPTEN_SHELL_HTML "${EMSCRIPTEN_SHELL_HTML}")

    #     if(CLICK_TO_START)
    #         string(CONCAT BODY_REPLACEMENT "
    # <body>
    #     <div id=\"overlay\">
    #         <p>Click to start game</p>
    #     </div>

    #     <script>
    #     window.addEventListener('load', function() {
    #         // disable events until clicked\n
    #         let eat = function(event){
    #             event.stopImmediatePropagation();
    #         };

    #         window.addEventListener('keydown', eat, true);
    #         window.addEventListener('keyup', eat, true);

    #         var overlay = document.getElementById('overlay');
    #         overlay.addEventListener('click', function() {
    #         overlay.style.display = 'none';
    #         window.removeEventListener('keydown', eat, true);
    #         window.removeEventListener('keyup', eat, true);
    #         });
    #     });
    #     </script>")
    #         string(REPLACE "<body>" "${BODY_REPLACEMENT}" EMSCRIPTEN_SHELL_HTML "${EMSCRIPTEN_SHELL_HTML}")
    #     endif()

    # endif()
    # file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shell.html.in "${EMSCRIPTEN_SHELL_HTML}")

    # set_target_properties("${TARGET_NAME}" PROPERTIES LINK_FLAGS "-s ASSERTIONS -s GL_DEBUG -s DEMANGLE_SUPPORT=1 -s OFFSCREEN_FRAMEBUFFER=1 -s ALLOW_MEMORY_GROWTH -s EXIT_RUNTIME=1 -s SDL2_IMAGE_FORMATS='[\"png\"]' --preload-file \"${CMAKE_SOURCE_DIR}/Assets\"@Assets --shell-file \"${CMAKE_CURRENT_BINARY_DIR}/shell.html.in\"")
else()
    target_link_libraries(${TARGET_NAME} PUBLIC
        SDL2::SDL2 # TODO: SDL is still needed for Input and color, could also be abstacted
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
    )
endif()



if(USE_STEAMWORKS)
    use_steamworks(${TARGET_NAME})
endif()

if(USE_VLD)
    if (WIN32)
        target_include_directories(${TARGET_NAME} PUBLIC ${VLD_INCLUDE_DIR})
        target_link_libraries(${TARGET_NAME} PUBLIC ${VLD_LIBRARY})
    endif()
endif()

if(NOT MSVC)
    if(USE_ADDRESS_SANITIZER)
        target_compile_options(${TARGET_NAME} PUBLIC -fsanitize=address)
        target_link_options(${TARGET_NAME} PUBLIC -fsanitize=address)
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

if(NOT EMSCRIPTEN)
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
endif()
