#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <mutex>
#include <cassert>

/////////////////////////////////////////////////////
// ��ɃT�u�V�X�e���̃C���X�^���X�쐬�̂��߂Ɏg�p����
/////////////////////////////////////////////////////
class SingletonFinalizer
{
public:
	typedef void(*FinalizerFunc)();

	static void addFinalizer(FinalizerFunc func);
	static void finalize();
};

template <typename T>
class singleton final
{
public:
	static T& GetInstance()
	{
		std::call_once(initFlag, create);
		assert(instance);
		return *instance;
	}

private:
	static std::once_flag initFlag;
	static T* instance;

	static void create()
	{
		instance = new T;
		SingletonFinalizer::addFinalizer(&singleton<T>::destroy);
	}

	static void destroy()
	{
		delete instance;
		instance = nullptr;
	}
};

template <typename T> std::once_flag singleton<T>::initFlag;
template <typename T> T* singleton<T>::instance = nullptr;

#endif // !SINGLETON_H_

