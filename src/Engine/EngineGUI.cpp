#include "EngineGUI.h"

#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <SceneManager.h>
#include <SDL_render.h>

#include "TweenEngine.h"


void jul::EngineGUI::Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(windowPtr, rendererPtr);
    ImGui_ImplSDLRenderer2_Init(rendererPtr);
}

void jul::EngineGUI::NewFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // TODO: Should be moved to a nice engine GUI dugger etc
    // This would be a nice project for in the future,
    // to make a nice engine GUI
    if(g_ShowDebugInfo)
    {
        ImGui::Text("Active Tweens:");
        const auto& tweens = TweenEngine::GetAllActiveTweens();

        for(auto&& tween : tweens)
        {
            std::string tweenText{};

            if(tween->GetTarget() == nullptr)
                tweenText = "Dead";
            else
                tweenText = tween->GetTarget()->GetName();

            if(tween->IsHalting())
                ImGui::PushStyleColor(ImGuiCol_SliderGrab,
                                      ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red color for halting tweens


            auto value = static_cast<float>(tween->GetTime());
            ImGui::SliderFloat(
                tweenText.c_str(),
                &value,
                0.0,
                static_cast<float>(tween->GetTween().duration) + static_cast<float>(tween->GetTween().delay));

            if(tween->IsHalting())
                ImGui::PopStyleColor();
        }
    }
}

void jul::EngineGUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

bool jul::EngineGUI::ProcessEvent(const SDL_Event* event) { return ImGui_ImplSDL2_ProcessEvent(event); }

void jul::EngineGUI::ShowDebugInfo(bool show) { g_ShowDebugInfo = show; }


void jul::EngineGUI::Destory()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
