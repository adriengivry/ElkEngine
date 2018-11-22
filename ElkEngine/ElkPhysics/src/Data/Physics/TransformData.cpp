#include "stdafx.h"

#include "ElkPhysics/Data/Physics/TransformData.h"

glm::vec3 ElkPhysics::Data::Physics::TransformData::GetWorldPosition()
{
	return localPosition;
	// return localPosition + (parent != nullptr ? parent->GetWorldPosition() : glm::vec3(0, 0, 0));
}

glm::vec3 ElkPhysics::Data::Physics::TransformData::GetWorldRotation()
{
	return localRotation;
	// return localRotation * (parent != nullptr ? parent->GetWorldRotation() : glm::vec3(1, 1, 1));
}

glm::vec3 ElkPhysics::Data::Physics::TransformData::GetWorldScale()
{
	return localScale;
	// return localScale * (parent != nullptr ? parent->GetWorldScale() : glm::vec3(1, 1, 1));
}
