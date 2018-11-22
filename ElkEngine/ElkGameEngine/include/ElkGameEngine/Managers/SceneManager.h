#pragma once

#include <ElkAPI/IManager.h>

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/Scene.h"
#include "ElkGameEngine/Objects/GameObject.h"
#include "ElkGameEngine/Utils/SharedContext.h"

namespace ElkGameEngine
{
	namespace Managers
	{
		/*
		* Manager to manipulate scenes
		*/
		class ELK_GAMEENGINE_EXPORT SceneManager : public ElkAPI::IManager
		{
		public:
			explicit SceneManager(ElkGameEngine::Utils::SharedContext& p_sharedContext);
			~SceneManager() = default;

			void Setup() override;
			void Close() override;

			void Update();

			void SetCurrentScene(const std::string& p_sceneName);
			ElkGameEngine::Objects::Scene& CreateScene(const std::string& p_sceneName, bool p_setCurrent = true);
			ElkGameEngine::Objects::GameObject& CreateEmptyGameObject(const std::string& p_name = "GameObject") const;
			ElkGameEngine::Objects::GameObject& CreateEmptyGameObject(ElkGameEngine::Objects::GameObject& p_parent, const std::string& p_name = "GameObject") const;
			ElkGameEngine::Objects::Scene& GetCurrentScene() const;

			bool HasCurrentScene() const;

		private:
			ElkGameEngine::Utils::SharedContext&									m_sharedContext;
			std::map<std::string, std::shared_ptr<ElkGameEngine::Objects::Scene>>	m_scenes;
			std::shared_ptr<Objects::Scene>											m_currentScene;
		};
	}
}
