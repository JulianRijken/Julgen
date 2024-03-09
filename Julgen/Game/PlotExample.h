#pragma once
#include <valarray>
#include <vector>

#include "Renderer.h"

namespace Examples
{

	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		Transform transform;
		int ID;
	};

	class GameObject3DAlt
	{
	public:
		Transform* transform; // We make the transform a pointer to keep it 4 bytes
		int ID;
	};

	double inline TransformForward_Pow2(double v, void*)
	{
		return std::pow(2, v);
	}

	double inline TransformInverse_Pow2(double v, void*)
	{
		return std::log2(v);
	}


	/// TODO: This is currently using the Renderer component
	///	In the future I would want to not have the renderer component exposed to the user
	///	and have it be a engine feature so that the user should use text renderer or sprite renderer
	///	For now I use it to make a quick example for plot rendering
	class PlotExample final : public jul::Renderer
	{
	public:
		PlotExample(jul::GameObject* parent, int renderLayer = 0);

	private:
		void UpdateGUI() override;

		int m_SampleCount = 10;

		static inline constexpr int BUFFER_SIZE = 10000000;
		std::vector<GameObject3D> m_LargeBufferGo{};
		std::vector<GameObject3DAlt> m_LargeBufferGoAlt{};

		std::vector<float> m_SampleDelayGo;
		std::vector<float> m_SampleStepSizeGo;

		std::vector<float> m_SampleDelayGoAlt;
		std::vector<float> m_SampleStepSizeGoAlt;
	};

}
