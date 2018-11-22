#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/Components/Behaviours/Physics/BoxCollider.h"

using namespace ElkGameEngine::Objects::Components::Behaviours::Physics;

BoxCollider::BoxCollider()
	: ACollider()
{
	SetName("BoxCollider");
}

ElkPhysics::Data::Colliders::AColliderData* BoxCollider::GetColliderData()
{
	return &m_boxColliderData;
}

const glm::vec3& BoxCollider::GetExtend()
{
	return m_boxColliderData.extend;
}

const glm::vec3 & BoxCollider::GetOffset()
{
	return m_boxColliderData.offset;
}

void BoxCollider::SetExtend(const glm::vec3 & p_value)
{
	m_boxColliderData.extend = p_value;
}

void BoxCollider::SetOffset(const glm::vec3 & p_value)
{
	m_boxColliderData.offset = p_value;
}
