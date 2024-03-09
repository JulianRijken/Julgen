#include "PlotExample.h"

#include <chrono>

#include <imgui.h>
#include <implot.h>

#include "implot_internal.h"

Examples::PlotExample::PlotExample(jul::GameObject* parent, int renderLayer) :
	Renderer(parent, "FpsCounter", renderLayer)
{
	m_LargeBufferGo = std::vector<GameObject3D>(BUFFER_SIZE);
	m_LargeBufferGoAlt = std::vector<GameObject3DAlt>(BUFFER_SIZE );
}


void Examples::PlotExample::UpdateGUI() 
{
	ImGui::Begin("Exercise 2");

	ImGui::Text(std::format("Buffer Size {}",BUFFER_SIZE).c_str());
	ImGui::InputInt("Sample Size", &m_SampleCount);

	if(ImGui::Button("Trash the cache with GameObject3D"))
	{
		m_SampleDelayGo.clear();
		m_SampleStepSizeGo.clear();

		for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
		{
			long long durationAverage{ 0 };
			for (int testIndex = 0; testIndex < m_SampleCount; ++testIndex)
			{
				const auto start = std::chrono::high_resolution_clock::now();

				// Perform the test
				for (int i = 0; i < BUFFER_SIZE; i += stepSize)
					m_LargeBufferGo[i].ID *= 2;

				// We discard the first and the last :) 
				if (testIndex == 0 or testIndex == m_SampleCount - 1)
					continue;

				durationAverage += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();;
			}

			durationAverage /= (m_SampleCount - 2);

			m_SampleStepSizeGo.push_back(static_cast<float>(stepSize));
			m_SampleDelayGo.push_back(static_cast<float>(durationAverage));
		}
	}

	if (not m_SampleStepSizeGo.empty() and not m_SampleDelayGo.empty())
	{
		if (ImPlot::BeginPlot("GameObject3D"))
		{
			ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxisScale(ImAxis_X1, TransformInverse_Pow2, TransformForward_Pow2);

			// Setup custom ticks
			const std::vector<double> m_Ticks(m_SampleStepSizeGo.begin(), m_SampleStepSizeGo.end());
			ImPlot::SetupAxisTicks(ImAxis_X1, m_Ticks.data(), static_cast<int>(m_SampleStepSizeGo.size()));

			ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
			ImPlot::PlotLine("GO", m_SampleStepSizeGo.data(), m_SampleDelayGo.data(), static_cast<int>(m_SampleStepSizeGo.size()));
			ImPlot::EndPlot();
		}
	}



	if (ImGui::Button("Trash the cache with GameObject3DAlt"))
	{
		m_SampleDelayGoAlt.clear();
		m_SampleStepSizeGoAlt.clear();

		for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
		{
			long long durationAverage{ 0 };
			for (int testIndex = 0; testIndex < m_SampleCount; ++testIndex)
			{
				const auto start = std::chrono::high_resolution_clock::now();

				// Perform the test
				for (int i = 0; i < BUFFER_SIZE; i += stepSize)
					m_LargeBufferGoAlt[i].ID *= 2;

				// We discard the first and the last :) 
				if (testIndex == 0 or testIndex == m_SampleCount - 1)
					continue;

				durationAverage += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();;
			}

			durationAverage /= (m_SampleCount - 2);

			m_SampleStepSizeGoAlt .push_back(static_cast<float>(stepSize));
			m_SampleDelayGoAlt.push_back(static_cast<float>(durationAverage));
		}
	}


	if (not m_SampleStepSizeGoAlt.empty() and not m_SampleDelayGoAlt.empty())
	{
		if (ImPlot::BeginPlot("GameObject3DAlt"))
		{
			ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxisScale(ImAxis_X1, TransformInverse_Pow2, TransformForward_Pow2);

			// Setup custom ticks
			const std::vector<double> m_Ticks(m_SampleStepSizeGoAlt.begin(), m_SampleStepSizeGoAlt.end());
			ImPlot::SetupAxisTicks(ImAxis_X1, m_Ticks.data(), static_cast<int>(m_SampleStepSizeGoAlt.size()));

			ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
			ImPlot::PlotLine("GO ALT", m_SampleStepSizeGoAlt.data(), m_SampleDelayGoAlt.data(), static_cast<int>(m_SampleStepSizeGoAlt.size()));

			ImPlot::EndPlot();
		}
	}


	if (not m_SampleStepSizeGoAlt.empty() and not m_SampleDelayGoAlt.empty() and not m_SampleStepSizeGo.empty() and not m_SampleDelayGo.empty())
	{
		if (ImPlot::BeginPlot("Combined"))
		{
			ImPlot::SetupAxes("Step", "Delay", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxisScale(ImAxis_X1, TransformInverse_Pow2, TransformForward_Pow2);

			// Setup custom ticks
			const std::vector<double> m_Ticks(m_SampleStepSizeGoAlt.begin(), m_SampleStepSizeGoAlt.end());
			ImPlot::SetupAxisTicks(ImAxis_X1, m_Ticks.data(), static_cast<int>(m_SampleStepSizeGoAlt.size()));

			ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
			ImPlot::PlotLine("GO", m_SampleStepSizeGo.data(), m_SampleDelayGo.data(), static_cast<int>(m_SampleStepSizeGo.size()));

			ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
			ImPlot::PlotLine("GO ALT", m_SampleStepSizeGoAlt.data(), m_SampleDelayGoAlt.data(), static_cast<int>(m_SampleStepSizeGoAlt.size()));


			ImPlot::EndPlot();
		}
	}

	ImGui::End();

}


