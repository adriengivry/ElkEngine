#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Data/Colliders/AColliderData.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Colliders
		{
			struct ELK_PHYSICS_EXPORT BoxColliderData : public AColliderData
			{
				BoxColliderData();
				~BoxColliderData() = default;

				glm::vec3 extend;
			};
		}
	}
}