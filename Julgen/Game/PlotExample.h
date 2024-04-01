#pragma once
#include <vector>
#include <chrono>

#include "RenderComponent.h"

namespace Examples
{

    // clang-format off
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};
    // clang-format on```

	class GameObject3D
	{
	public:
		Transform transform;
		int id;
	};

	class GameObject3DAlt
	{
	public:
		Transform* transform; // We make the transform a pointer to keep it 4 bytes
		int id;
	};

	/// TODO: This is currently using the Renderer component
	///	In the future I would want to not have the renderer component exposed to the user
	///	and have it be a engine feature so that the user should use text renderer or sprite renderer
	///	For now I use it to make a quick example for plot rendering
    class PlotExample final : public jul::RenderComponent
	{
	public:
		PlotExample(jul::GameObject* parent, int renderLayer = 0);

	private:
		void UpdateGUI() override;

        template<typename TestType>
        void PerformTest(std::vector<TestType>& buffer,std::vector<float>& samples)
        {
            samples.clear();

            for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
            {
                long long durationAverage{ 0 };
                for (int testIndex = 0; testIndex < m_SampleCount; ++testIndex)
                {
                    const auto START = std::chrono::high_resolution_clock::now();

                    // Perform the test
                    for (int i = 0; i < BUFFER_SIZE; i += stepSize)
                        buffer[i].id *= 2;

                    durationAverage += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - START).count();;
                }

                durationAverage /= (m_SampleCount);

                samples.push_back(static_cast<float>(durationAverage));
            }
        }

		int m_SampleCount = 10;

		static inline constexpr int BUFFER_SIZE = 10000000;
		std::vector<GameObject3D> m_LargeBufferGo{};
		std::vector<GameObject3DAlt> m_LargeBufferGoAlt{};

		std::vector<float> m_SampleDelayGo;
		std::vector<float> m_SampleDelayGoAlt;

        // This is kinda stupid but needed for the plotting graphs
		static inline const float s_SampleSteps[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
		static inline const double s_GraphTicks[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
		static inline const int s_TickCount = 11;
	};

}
