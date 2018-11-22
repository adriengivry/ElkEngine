#include "stdafxGameEngine.h"

#include "ElkGameEngine/Objects/Components/Behaviours/Physics/Rigidbody.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/SphereCollider.h"

using namespace ElkGameEngine::Objects::Components::Behaviours::Physics;

ElkGameEngine::Objects::Components::Behaviours::Physics::SphereCollider::SphereCollider() : ACollider()
{
	SetName("SphereCollider");
}
