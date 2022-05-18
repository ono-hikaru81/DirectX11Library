
#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

namespace Utility
{
	template<class T>
	class Singleton
	{
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	public:
		static std::weak_ptr<T> Instance()
		{
			if (!instance)
			{
				instance.reset(new T());
			}
		}

	private:
		static std::shared_ptr<T> instance;
	};

	template<class T>
	std::shared_ptr<T> Singleton<T>::instance = nullptr;
}

#endif // !SINGLETON_H
