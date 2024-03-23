#include "PlotExample.h"

#include <fmt/core.h>
#include <imgui.h>
#include <implot.h>

Examples::PlotExample::PlotExample(jul::GameObject* parent, int renderLayer) :
      RenderComponent(parent, "FpsCounter", renderLayer)
{
	m_LargeBufferGo = std::vector<GameObject3D>(BUFFER_SIZE);
	m_LargeBufferGoAlt = std::vector<GameObject3DAlt>(BUFFER_SIZE);
}


void Examples::PlotExample::UpdateGUI()
{
    ImGui::Begin("Exercise 2");

    ImGui::Text("%s", fmt::format("Buffer Size {}", BUFFER_SIZE).c_str());
    ImGui::InputInt("Sample Size", &m_SampleCount);


	if (ImGui::Button("Trash the cache with GameObject3D"))
		PerformTest(m_LargeBufferGo, m_SampleDelayGo);


	if (not m_SampleDelayGo.empty())
	{
		if(ImPlot::BeginPlot("GameObject3D")) {

            ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
            ImPlot::SetupAxisTicks(ImAxis_X1, s_GraphTicks, s_TickCount);

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
            ImPlot::PlotLine("GO", s_SampleSteps, m_SampleDelayGo.data(), s_TickCount);

            ImPlot::EndPlot();
        }
	}



	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		PerformTest(m_LargeBufferGoAlt, m_SampleDelayGoAlt);




	if (not m_SampleDelayGoAlt.empty())
	{
		if(ImPlot::BeginPlot("GameObject3DAlt"))
        {
            ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
            ImPlot::SetupAxisTicks(ImAxis_X1, s_GraphTicks, s_TickCount);

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
            ImPlot::PlotLine("GO ALT", s_SampleSteps, m_SampleDelayGoAlt.data(), s_TickCount);

            ImPlot::EndPlot();
        }
	}


	if (not m_SampleDelayGoAlt.empty() and not m_SampleDelayGo.empty())
	{
		if(ImPlot::BeginPlot("Combined")) {

            ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
            ImPlot::SetupAxisTicks(ImAxis_X1, s_GraphTicks, s_TickCount);

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
            ImPlot::PlotLine("GO", s_SampleSteps, m_SampleDelayGo.data(), s_TickCount);

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
            ImPlot::PlotLine("GO ALT", s_SampleSteps, m_SampleDelayGoAlt.data(), s_TickCount);

            ImPlot::EndPlot();
        }

	}

	ImGui::End();
}



