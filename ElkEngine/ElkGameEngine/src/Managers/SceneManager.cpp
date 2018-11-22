#include "stdafxGameEngine.h"

#include "ElkGameEngine/Managers/SceneManager.h"

using namespace ElkGameEngine::Managers;
using namespace ElkGameEngine::Objects;

SceneManager::SceneManager(ElkGameEngine::Utils::SharedContext& p_sharedContext)
	: m_sharedContext(p_sharedContext)
{
	SceneManager::Setup();
}

void SceneManager::Update() {}
void SceneManager::Close() {}

void SceneManager::Setup()
{
	m_currentScene = nullptr;
}

void SceneManager::SetCurrentScene(const std::string& p_sceneName)
{
	for (const auto& scene : m_scenes)
		if (scene.first == p_sceneName)
			m_currentScene = scene.second;
}

Scene& SceneManager::CreateScene(const std::string& p_sceneName, const bool p_setCurrent)
{
	auto newScene = std::make_shared<Scene>(m_sharedContext);
	newScene->SetName(p_sceneName);

	m_scenes.insert_or_assign(p_sceneName, newScene);
	if (p_setCurrent)
		m_currentScene = std::shared_ptr<Scene>(newScene);

	return *newScene.get();
}

GameObject& SceneManager::CreateEmptyGameObject(const std::string& p_name) const
{
	GameObject* gameObject = new GameObject(m_sharedContext, m_currentScene->GetRoot(), p_name);
	return *gameObject;
}

GameObject& SceneManager::CreateEmptyGameObject(GameObject& p_parent, const std::string& p_name) const
{
	GameObject* gameObject = new GameObject(m_sharedContext, p_parent, p_name);
	return *gameObject;
}

Scene& SceneManager::GetCurrentScene() const
{
	return *m_currentScene.get();
}

bool SceneManager::HasCurrentScene() const
{
	if (m_currentScene)
		return true;
	else
		return false;
}