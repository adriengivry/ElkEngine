#include "stdafx.h"

#include "ElkPhysics/Data/Colliders/CollisionData.h"

using namespace ElkPhysics::Data::Colliders;

CollisionData::CollisionData(const glm::vec3& p_direction, const bool p_colliding)
{
	colliding = p_colliding;
	direction = p_direction;
	distance = glm::length(direction);
}