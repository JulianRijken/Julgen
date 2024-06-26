#ifndef SINGLETON_H
#define SINGLETON_H

namespace jul
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
            static T instance{};
            return instance;
        }

        virtual ~Singleton() = default;

		Singleton(Singleton&& other) = delete;
		Singleton(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;

	protected:
		Singleton() = default;
	};
}  // namespace jul
#endif // SINGLETON_H