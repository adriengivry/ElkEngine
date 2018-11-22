#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Math/Math.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Colliders
		{
			struct ELK_PHYSICS_EXPORT CollisionData
			{
			public:
				CollisionData(const glm::vec3& p_direction, const bool p_colliding);
				~CollisionData() = default;

				float distance;
				glm::vec3 direction;	
				bool colliding;
			};
		}
	}
}