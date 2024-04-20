#pragma once

#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace jul
{
    /**
     * Simple RAII wrapper for the ImGUI renderer
     */
class EngineGUI final : public Singleton<EngineGUI>
    {
    public:
        void Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);
        void Destroy();
        void NewFrame() const;
        void EndFrame() const;
    };
}

