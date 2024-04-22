#pragma once


struct SDL_Window;
struct SDL_Renderer;

namespace jul
{
    class EngineGUI final
    {
    public:
        EngineGUI() = delete;

        static void Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);
        static void Destroy();
        static void NewFrame();
        static void EndFrame();
    };
}

