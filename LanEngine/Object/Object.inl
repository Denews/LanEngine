#include "Object.h"
#include <typeinfo>
#include "../Utils/Utils.h"

namespace Lan
{
	template<typename T, typename... Args>
	T* Object::AddComponent(Args&&... args)
	{
		if (IsComponentExist<T>())
		{
			LOG(LogLevel::Warning, "�ִ� ������Ʈ�� �ٽ� �߰��Ϸ��� �õ���");
			return dynamic_cast<T *>(m_Components[typeid(T).name()]);
		}

		T* component = new T(this, args...);

		m_Components.insert(std::make_pair(typeid(T).name(), component));
		return component;
	}

	template <typename T>
	void Object::RemoveComponent()
	{
		if (Object::IsComponentExist<T>()) m_GarbageCollector.push_back(m_Components[typeid(T).name]);
		else LOG(LogLevel::Warning, "���� ������Ʈ�� ���� �õ���");
	}

	template <typename T>
	T* Object::GetComponent()
	{
		if (Object::IsComponentExist<T>()) return dynamic_cast<T*>(m_Components.at(typeid(T).name()));
		else LOG(LogLevel::Warning, "���� ������Ʈ�� �˻� �õ���");

		return nullptr;
	}

	template <typename T>
	bool Object::IsComponentExist()
	{
		if (m_Components.count(typeid(T).name()) > 0) return true;
		else return false;
	}

}