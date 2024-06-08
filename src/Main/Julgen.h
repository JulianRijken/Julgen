#pragma once

struct SDL_Window;

namespace jul
{
	class Julgen final
	{

    public:
        // Why? Well it is quite nice to be able to assert or check
        // what loop state the engine is in. For instance, some things
        // can only happend during some parts like destroying objects.
        // This allows us to keep track of that the engine is doing
        enum class LoopState
        {
            GameTimeUpdate,
            ProcessInput,
            FixedUpdateActive,
            FixedUpdate,
            FixedUpdateWorld,
            Update,
            MessageDispatch,
            TweenUpdate,
            LateUpdate,
            Render,
            MarkScenesForUnload,
            MarkGameObjectsForDestroy,
            CleanupGameObjects,
            CleanupScenes,
            LoadScenesSetToLoad,
            AddingFrameCount,
        };

        explicit Julgen();
        ~Julgen();

        Julgen(Julgen&&) = delete;
        Julgen(const Julgen&) = delete;
        Julgen& operator=(Julgen&&) = delete;
        Julgen& operator=(const Julgen&) = delete;

        inline static LoopState g_LoopState{};

        void Run();
        void RunOneFrame();

        void PreInit();
        void GameStart();


    private:
        double m_Lag{};
        bool m_IsApplicationQuitting{};
        SDL_Window* m_Window;
    };

}  // namespace jul
