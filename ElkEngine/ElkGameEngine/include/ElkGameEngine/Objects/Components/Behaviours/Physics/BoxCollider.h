#pragma once

#include <ElkPhysics/Data/Colliders/BoxColliderData.h>

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/ACollider.h"

namespace ElkGameEngine
{
	namespace Objects
	{
		namespace Components
		{
			namespace Behaviours
			{
				namespace Physics
				{
					class ELK_GAMEENGINE_EXPORT BoxCollider: public Objects::Components::Behaviours::Physics::ACollider
					{
					public:
						BoxCollider();

						virtual ElkPhysics::Data::Colliders::AColliderData* GetColliderData();

						const glm::vec3& GetExtend();
						const glm::vec3& GetOffset();
						void SetExtend(const glm::vec3& p_value);
						void SetOffset(const glm::vec3& p_value);

					private:
						ElkPhysics::Data::Colliders::BoxColliderData m_boxColliderData;
					};
				}
			}
		}
	}
}