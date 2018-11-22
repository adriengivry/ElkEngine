#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Math/Math.h"
#include "ElkPhysics/Data/Colliders/AColliderData.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Physics
		{
			struct ELK_PHYSICS_EXPORT RigidbodyData
			{
				RigidbodyData() = default;
				~RigidbodyData() = default;

				glm::vec3 velocity;
			};
		}
	}
}