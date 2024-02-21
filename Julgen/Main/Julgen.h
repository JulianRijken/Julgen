#pragma once

namespace jul
{
	class Julgen final
	{
		bool m_IsApplicationQuitting{};

	public:

		explicit Julgen();
		~Julgen();

		void Run();

		Julgen(Julgen&& other) = delete;
		Julgen(const Julgen& other) = delete;
		Julgen& operator=(Julgen&& other) = delete;
		Julgen& operator=(const Julgen& other) = delete;
	};
}