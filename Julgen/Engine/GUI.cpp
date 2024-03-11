#include "GUI.h"

#include <SDL_render.h>


#include <imgui.h>
#include <implot.h>

#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

void jul::GUI::Initialize(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(windowPtr, rendererPtr);
    ImGui_ImplSDLRenderer2_Init(rendererPtr);
}

void jul::GUI::NewFrame() const
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void jul::GUI::EndFrame() const
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}



void jul::GUI::Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}
