#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Math/Math.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Physics
		{
			struct ELK_PHYSICS_EXPORT TransformData
			{
				TransformData* parent = nullptr;

				glm::vec3 localPosition;
				glm::vec3 localRotation;
				glm::vec3 localScale;

				glm::vec3 GetWorldPosition();
				glm::vec3 GetWorldRotation();
				glm::vec3 GetWorldScale();
			};
		}
	}
}