#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/Scene.h"

using namespace ElkGameEngine::Objects;

Scene::Scene(ElkGameEngine::Utils::SharedContext& p_sharedContext)
	: m_sharedContext(p_sharedContext), m_root(std::make_shared<Objects::GameObject>(m_sharedContext))
{
	SetName("Scene");
}

GameObject& Scene::GetRoot() const
{
	return *m_root;
}