#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/GameObject.h"
#include "ElkGameEngine/Utils/SharedContext.h"
#include "ElkGameEngine/Objects/Components/Transform.h"

using namespace ElkGameEngine;

Objects::GameObject::GameObject(ElkGameEngine::Utils::SharedContext& p_sharedContext, Objects::GameObject& p_parent, const std::string& p_name)
	: m_sharedContext(p_sharedContext)
{
	SetName(p_name);

	Setup();

	SetParent(p_parent);

	p_parent.AddChild(*this);
}

void ElkGameEngine::Objects::GameObject::Setup()
{
	transform = AddComponent<Objects::Components::Transform>();
}

Objects::GameObject::GameObject(ElkGameEngine::Utils::SharedContext& p_sharedContext)
	: m_sharedContext(p_sharedContext)
{
	SetName("Scene Root");

	m_parent = nullptr;

	Setup();

	GetComponent<Objects::Components::Transform>()->SetScale(glm::vec3(1, 1, 1));
}

void Objects::GameObject::AddChild(Objects::GameObject& p_child)
{
	m_childs.push_back(std::shared_ptr<Objects::GameObject>(&p_child));
}

std::unordered_map<size_t, std::shared_ptr<Objects::Components::AComponent>>& Objects::GameObject::GetComponents()
{
	return m_components;
}

std::vector<std::shared_ptr<Objects::GameObject>>& Objects::GameObject::GetChilds()
{
	return m_childs;
}

Objects::GameObject& Objects::GameObject::GetParent() const
{
	return *m_parent;
}

bool Objects::GameObject::HasParent() const
{
	return m_parent != nullptr;
}

void Objects::GameObject::SetParent(Objects::GameObject& p_parent)
{
	m_parent = &p_parent;
	transform->SetParent(*p_parent.transform);
}
