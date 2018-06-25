#include "../Defines.h"
#include "Singleton.h"
#include "Logger.h"

namespace Lan
{
	template<typename T>
	T* Lan::Singleton<T>::s_Instance = nullptr;

	template<typename T>
	void Singleton<T>::CreateInstance()
	{
		if (s_Instance != nullptr)
		{
			LOG(LogLevel::Warning, "�̹� �ʱ�ȭ�� �̱��� ��ü �ʱ�ȭ �õ�");
		}
		else
		{
			s_Instance = new T();
		}
	}

	template<typename T>
	T& Singleton<T>::GetInstance()
	{
		if (s_Instance == nullptr)
		{
			LOG(LogLevel::Warning, "���ʱ�ȭ ������ �̱��� ��ü ���� �õ�. CreateInstance() �Լ��� ���� ȣ���Ͻÿ�");
			s_Instance = new T();
		}

		return *s_Instance;
	}

	template<typename T>
	void Singleton<T>::DestroyInstance()
	{
		if (s_Instance == nullptr)
		{
			LOG(LogLevel::Warning, "���ʱ�ȭ ������ �̱��� ��ü �Ҵ� ���� �õ�. CreateInstance() �Լ��� ���� ȣ���Ͻÿ�");
		}
		else
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}
}