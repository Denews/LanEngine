#include "Defines.h"
#include "Object.h"
#include "../Component/Component.h"
#include "../Component/Transform.h"
#include <utility>
#include <map>
#include <iterator>
#include "../Graphics/GraphicsManager.h"

using namespace std;

namespace Lan
{
	Object::Object():
		m_IsActive(true),
		m_IsVisible(true),
		m_Scene(nullptr),
		m_Parent(nullptr)
	{
		addComponent<Transform>();
	}

	Object::~Object()
	{
		for (Object* o : m_Children)
		{
			delete o;
		}
		m_Children.clear();
		auto it = m_Components.begin();
		for(; it != m_Components.end(); it++)
		{
			delete it->second;
		}
		m_Components.clear();

	}

	Object::Garbage::Garbage(Object::GarbageType type, void* element) :
		type(type),
		element(element)
	{

	}

	bool Object::isActive() const
	{
		return m_IsActive;
	}

	bool Object::isVisible() const
	{
		return m_IsVisible;
	}

	Scene& Object::getScene() const
	{
		return *m_Scene;
	}

	void Object::update(Context& context)
	{
		collectGarbage();

		if (!m_IsActive) return;

		onUpdate(context);

		for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
		{
			iter->second->update(context);
		}

		for (Object* child : m_Children)
		{
			child->update(context);
		}
	}

	void Object::draw()
	{
		if (!m_IsVisible) return;

		ID3D11DeviceContext * deviceContext = GraphicsManager::getInstance().getDeviceContext();

		onDraw();

		for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
		{
			iter->second->draw();
		}

		deviceContext->DrawIndexed(6, 0, 0);

		for (Object* child : m_Children)
		{
			child->draw();
		}
	}

	Transform& Object::getTransform()
	{
		return *(getComponent<Transform>());
	}

	void Object::addChild(Object& object)
	{
		if (m_Children.count(&object) > 0)
		{
			LOG(LogLevel::Warning, "있는 차일드를 다시 추가하려고 시도함");
			return;
		}

		m_Children.insert(&object);
	}

	void Object::removeChild(Object& object)
	{
		if (m_Children.count(&object) < 1)
		{
			LOG(LogLevel::Warning, "없는 차일드를 삭제하려고 시도함");
			return;
		}

		m_GarbageCollector.push_back(Garbage(GarbageType::ObjectType, static_cast<void*>(&object)));
	}

	const std::set<Object*>& Object::getChildren() const
	{
		return m_Children;
	}

	bool Object::isChildExist(Object& object)
	{
		if (m_Children.count(&object) > 0) return true;
		else return false;
	}

	void Object::setActive(bool active)
	{
		m_IsActive = active;
	}

	void Object::setVisible(bool visible)
	{
		m_IsVisible = visible;
	}

	void Object::setScene(Scene* scene)
	{
		m_Scene = scene;
	}

	void Object::setParent(Object* object)
	{
		m_Parent = object;
	}

	tsize Object::getChildCount() const
	{
		return m_Children.size();
	}

	void Object::collectGarbage()
	{
		for (Garbage garbage : m_GarbageCollector)
		{
			Component* component = reinterpret_cast<Component *>(garbage.element);
			Object* object = reinterpret_cast<Object *>(garbage.element);
			switch (garbage.type)
			{
			case GarbageType::ComponentType:
				if (m_Components.count(typeid(component).name()) < 1)
				{
					LOG(LogLevel::Error, "컴포넌트제거 오류");
					break;
				}

				m_Components.erase(typeid(component).name());
				delete component;
				break;
			case GarbageType::ObjectType:
				if (isChildExist(*object))
				{
					LOG(LogLevel::Error, "차일드 제거 오류");
					break;
				}

				m_Children.erase(object);
				delete object;
				break;
			}
		}

		m_GarbageCollector.clear();
	}

}