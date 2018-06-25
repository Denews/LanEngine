#include "SceneManager.h"

namespace Lan
{
	template<typename T, typename... Args>
	bool SceneManager::AddScene(tstring name, Args&&... args)
	{
		if (!IsSceneExist(name))
		{
			Scene* scene = new T(args...);
			m_Scenes.insert(make_pair(name, scene));
			return true;
		}
		else
		{
			LOG(LogLevel::Warning, "�̹� �����ϴ� Scene�� �߰� �õ�");
			return false;
		}
	}
}