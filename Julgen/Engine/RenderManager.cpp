#include "RenderManager.h"

#include <stdexcept>
#include <cstring>
#include <algorithm>

#include "GUI.h"
#include "imgui.h"

#include "Renderer.h"
#include "SceneManager.h"

#include "Texture2D.h"
#include "Transform.h"


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void jul::RenderManager::Initialize(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_Renderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void jul::RenderManager::Render()
{
	GUI::GetInstance().NewFrame();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	RenderGUI();
	RenderObjects();

	GUI::GetInstance().EndFrame();
	
	SDL_RenderPresent(m_Renderer);
}

void jul::RenderManager::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void jul::RenderManager::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void jul::RenderManager::RenderObjects() const
{
	auto renderers = std::vector(s_GlobalRendererPtrs.begin(), s_GlobalRendererPtrs.end());

	const auto compareDistance = [](const Renderer* a, const Renderer* b)
		{
			return a->Transform().WorldPosition().z > b->Transform().WorldPosition().z;
		};

	const auto compareLayer = [](const Renderer* a, const Renderer* b)
		{
			return a->GetRenderLayer() < b->GetRenderLayer();
		};

	std::ranges::sort(renderers, compareDistance);
	std::ranges::stable_sort(renderers, compareLayer);

	for (const Renderer* renderer : renderers)
		renderer->Render();
}

void jul::RenderManager::RenderGUI()
{
	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Generate samples and plot them
	float samples[100];
	for (int n = 0; n < 100; n++)
		samples[n] = sinf((float)n * 0.2f + (float)ImGui::GetTime() * 1.5f);
	ImGui::PlotLines("Samples", samples, 100);

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}
