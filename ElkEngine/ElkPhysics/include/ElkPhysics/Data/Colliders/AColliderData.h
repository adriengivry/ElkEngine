#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Math/Math.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Colliders
		{
			struct ELK_PHYSICS_EXPORT AColliderData
			{
				enum class ColliderType {BOX, SPHERE};
				AColliderData(ColliderType p_type);
				virtual ~AColliderData() = default;

				ColliderType type;
				glm::vec3 offset;
			};
		}
	}
}