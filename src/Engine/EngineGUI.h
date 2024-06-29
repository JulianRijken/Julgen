#ifndef ENGINEGUI_H
#define ENGINEGUI_H

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

namespace jul
{
    class EngineGUI final
    {
    public:
        EngineGUI() = delete;

        static void Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);
        static void Destory();
        static void NewFrame();
        static void EndFrame();
        static bool ProcessEvent(const SDL_Event* event);

        static void ShowDebugInfo(bool show);

    private:
        static inline bool g_ShowDebugInfo{ false };
    };
}

#endif // ENGINEGUI_H