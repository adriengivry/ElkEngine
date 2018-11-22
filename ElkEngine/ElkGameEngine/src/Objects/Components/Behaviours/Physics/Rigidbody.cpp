#include "stdafxGameEngine.h"

#include <ElkPhysics/Data/Colliders/BoxColliderData.h>

#include "ElkGameEngine/Objects/Components/Behaviours/Physics/Rigidbody.h"

using namespace ElkGameEngine::Objects::Components::Behaviours::Physics;

ElkGameEngine::Objects::Components::Behaviours::Physics::Rigidbody::Rigidbody() 
	: ABehaviour()
{
	SetName("Rigidbody");
}

const glm::vec3 & Rigidbody::GetVelocity()
{
	return m_rigidbodyData.velocity;
}

void Rigidbody::SetVelocity(const glm::vec3 & p_value)
{
	m_rigidbodyData.velocity = p_value;
}
