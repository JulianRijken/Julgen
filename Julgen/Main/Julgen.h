#pragma once

namespace jul
{


	class Julgen final
	{
		double m_Lag{};
		bool m_IsApplicationQuitting{};

	public:

		explicit Julgen();
		~Julgen();

        Julgen(Julgen&&) = delete;
        Julgen(const Julgen&) = delete;
        Julgen& operator=(Julgen&&) = delete;
        Julgen& operator=(const Julgen&) = delete;

		void Run();
		void RunOneFrame();
	};
}
