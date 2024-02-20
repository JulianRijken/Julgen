#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class Julgen final
	{
		bool m_quit{};

	public:

		explicit Julgen(const std::filesystem::path& dataPath);
		~Julgen();

		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Julgen(Julgen&& other) = delete;
		Julgen(const Julgen& other) = delete;
		Julgen& operator=(Julgen&& other) = delete;
		Julgen& operator=(const Julgen& other) = delete;
	};
}