#pragma once

#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace jul
{
    /**
     * Simple RAII wrapper for the ImGUI renderer
     */
    class GUI final : public Singleton<GUI>
    {
    public:
        void Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);
        void NewFrame() const;
        void EndFrame() const;
        void Destroy();
    };
}

