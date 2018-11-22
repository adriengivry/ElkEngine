#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/Components/Transform.h"
#include "ElkGameEngine/Objects/GameObject.h"

using namespace ElkGameEngine::Objects::Components;

Transform::Transform()
{
	SetName("Transform");

	m_transformData.localPosition = glm::vec3(0, 0, 0);
	m_transformData.localRotation = glm::vec3(0, 0, 0);
	m_transformData.localScale = glm::vec3(1, 1, 1);
}

Transform::Transform(const glm::vec3& p_position)
{
	m_transformData.localPosition = p_position;
	m_transformData.localRotation = glm::vec3(0, 0, 0);
	m_transformData.localScale = glm::vec3(1, 1, 1);
}

Transform::Transform(const glm::vec3& p_position, const glm::vec3& p_rotation)
{
	m_transformData.localPosition = p_position;
	m_transformData.localRotation = p_rotation;
	m_transformData.localScale = glm::vec3(1, 1, 1);
}

Transform::Transform(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale)
{
	m_transformData.localPosition = p_position;
	m_transformData.localRotation = p_rotation;
	m_transformData.localScale = p_scale;
}

glm::vec3 ElkGameEngine::Objects::Components::Transform::GetWorldPosition()
{
	return m_transformData.GetWorldPosition();
}

glm::vec3 ElkGameEngine::Objects::Components::Transform::GetWorldRotation()
{
	return m_transformData.GetWorldRotation();
}

glm::vec3 ElkGameEngine::Objects::Components::Transform::GetWorldScale()
{
	return m_transformData.GetWorldScale();
}

glm::vec3& Transform::GetPosition()
{
	return GetLocalPosition();
}

glm::vec3& Transform::GetRotation()
{
	return GetLocalRotation();
}

glm::vec3& Transform::GetScale()
{
	return GetLocalScale();
}

glm::vec3& ElkGameEngine::Objects::Components::Transform::GetLocalPosition()
{
	return m_transformData.localPosition;
}

glm::vec3& ElkGameEngine::Objects::Components::Transform::GetLocalRotation()
{
	return m_transformData.localRotation;
}

glm::vec3& ElkGameEngine::Objects::Components::Transform::GetLocalScale()
{
	return m_transformData.localScale;
}

void Transform::SetPosition(const glm::vec3& p_position)
{
	m_transformData.localPosition = p_position;
}

void Transform::SetRotation(const glm::vec3& p_rotation)
{
	m_transformData.localRotation = p_rotation;
}

void Transform::SetScale(const glm::vec3& p_scale)
{
	m_transformData.localScale = p_scale;
}

void ElkGameEngine::Objects::Components::Transform::SetParent(Transform & p_parent)
{
	m_transformData.parent = &p_parent.GetTransformData();
}

void Transform::Translate(const glm::vec3& p_translate)
{
	m_transformData.localPosition += p_translate;
}

void Transform::Rotate(const glm::vec3& p_rotate)
{
	m_transformData.localRotation += p_rotate;
}

void Transform::Scale(const glm::vec3& p_scale)
{
	m_transformData.localScale *= p_scale;
}
