#pragma once

#include <ElkPhysics/Data/Physics/RigidBodyData.h>

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ABehaviour.h"

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
					class ELK_GAMEENGINE_EXPORT Rigidbody : public Objects::Components::Behaviours::ABehaviour
					{
					public:
						Rigidbody();

						ElkPhysics::Data::Physics::RigidbodyData* GetRigidbodyData() { return &m_rigidbodyData; }

						const glm::vec3& GetVelocity();
						void SetVelocity(const glm::vec3& p_value);

					private:
						ElkPhysics::Data::Physics::RigidbodyData m_rigidbodyData;
					};
				}
			}
		}
	}
}
