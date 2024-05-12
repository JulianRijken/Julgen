
# Credits to Matias https://allpurposem.at/ for providing help with steamworks setup
function(use_steamworks ENGINE_TARGET)
    set(STEAMWORKS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks")

    if(NOT EXISTS "${STEAMWORKS_DIR}")
        message(
            FATAL_ERROR
                "Steamworks directory not found. Please place the contents of the Steamworks zip file at external/steamworks"
        )
    else()
        file(GLOB STEAMWORKS_FILES "${STEAMWORKS_DIR}/*")
        if(NOT STEAMWORKS_FILES)
            message(
                FATAL_ERROR
                    "Steamworks directory ${STEAMWORKS_DIR} is empty. Please make sure it contains the necessary files."
            )
        endif()
    endif()

    set(STEAMWORKS_INCLUDE_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks/public/steam)
    set(STEAMWORKS_LIB_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/external/steamworks/redistributable_bin)

    if(WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/win64/steam_api64.lib)

            add_custom_command(
                TARGET ${ENGINE_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                                ${STEAMWORKS_LIB_DIR}/win64/steam_api64.dll ${CMAKE_BINARY_DIR})

        else()
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/steam_api.lib)

            add_custom_command(
                TARGET ${ENGINE_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${STEAMWORKS_LIB_DIR}/steam_api.dll
                                ${CMAKE_BINARY_DIR})
        endif()

    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/linux64/libsteam_api.so)
        else()
            set(STEAMWORKS_LIB ${STEAMWORKS_LIB_DIR}/linux32/libsteam_api.so)
        endif()
    endif()

    target_include_directories(${ENGINE_TARGET} PUBLIC ${STEAMWORKS_INCLUDE_DIR})
    target_link_libraries(${ENGINE_TARGET} PUBLIC ${STEAMWORKS_LIB})

    target_compile_definitions(${ENGINE_TARGET} PUBLIC USE_STEAMWORKS)
endfunction()




function(make_libs_available)

    set(FETCHCONTENT_QUIET OFF)

    include(FetchContent)

    if(NOT WIN32)
        find_package(glm REQUIRED)
        find_package(fmt REQUIRED)

        find_package(SDL2 REQUIRED)
        find_package(SDL2_ttf REQUIRED)
        find_package(SDL2_image REQUIRED)
    else()

        message(STATUS "Downloading " glm...)
        FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm
            GIT_TAG 1.0.1
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(glm)

        message(STATUS "Downloading " fmt...)
        FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG 10.2.1
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(fmt)

        message(STATUS "Downloading " sdl2...)

        set(SDL_STATIC OFF)
        FetchContent_Declare(
            sdl2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG release-2.30.2
            GIT_SHALLOW TRUE
            OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2)

        set(SDL2IMAGE_JXL ON)
        set(SDL2IMAGE_VENDORED ON)
        message(STATUS "Downloading " sdl2_image...)
        FetchContent_Declare(
            sdl2-image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
            GIT_TAG release-2.8.2
            GIT_SHALLOW TRUE
            OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2-image)

        # Windows requires building FreeType
        set(SDL2TTF_VENDORED ON)

        message(STATUS "Downloading " sdl2_ttf...)
        FetchContent_Declare(
            sdl2-ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
            GIT_TAG release-2.22.0
            GIT_SHALLOW TRUE
            OVERRIDE_FIND_PACKAGE TRUE)
        FetchContent_MakeAvailable(sdl2-ttf)
    endif()


    # Add and Link box2D
    # TODO: Set to public, idealy private, the game does not need to know about box2D
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/Box2D)


    # Setup soloud
    # Again credits to mat for providing submodule
    set(SOLOUD_BACKEND_NULL OFF)
    set(SOLOUD_BACKEND_WINMM OFF)
    set(SOLOUD_BACKEND_MINIAUDIO ON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/soloud)
    if(NOT MSVC)
        target_compile_options(soloud PRIVATE -Wno-multichar)
    endif()

endfunction()

function(make_vld_available)
    set(VLD_DIR "$ENV{ProgramFiles\(x86\)}/Visual Leak Detector")

    if(EXISTS ${VLD_DIR})
        set(VLD_INCLUDE_DIR "${VLD_DIR}/include" PARENT_SCOPE)

        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(VLD_LIBRARY "${VLD_DIR}/lib/Win64/vld.lib" PARENT_SCOPE)
        else()
            set(VLD_LIBRARY "${VLD_DIR}/lib/Win32/vld.lib" PARENT_SCOPE)
        endif()
    endif()
endfunction()
