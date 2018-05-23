#include "Object.h"
#include <typeinfo>
#include "../Utils/Utils.h"

namespace Lan
{
	template <typename T>
	void Object::removeComponent()
	{
		if (Object::isComponentExist<T>()) m_GarbageCollector.push_back(Garbage(GarbageType::ComponentType, static_cast<void *>(m_Components[typeid(T).name])));
		else LOG(LogLevel::Warning, "���� ������Ʈ�� ���� �õ���");
	}

	template <typename T>
	T* Object::getComponent()
	{
		if (Object::isComponentExist<T>()) return dynamic_cast<T*>(m_Components.at(typeid(T).name()));
		else LOG(LogLevel::Warning, "���� ������Ʈ�� �˻� �õ���");

		return nullptr;
	}

	template <typename T>
	bool Object::isComponentExist()
	{
		if (m_Components.count(typeid(T).name()) > 0) return true;
		else return false;
	}

}