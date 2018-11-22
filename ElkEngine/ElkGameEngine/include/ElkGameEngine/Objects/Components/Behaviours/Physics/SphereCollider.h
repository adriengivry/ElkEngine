#pragma once

#include <ElkPhysics/Data/Colliders/SphereColliderData.h>

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
					class ELK_GAMEENGINE_EXPORT SphereCollider : public Objects::Components::Behaviours::Physics::ACollider
					{
					public:
						SphereCollider();

						virtual ElkPhysics::Data::Colliders::AColliderData* GetColliderData() { return &m_sphereColliderData; }

					private:
						ElkPhysics::Data::Colliders::SphereColliderData m_sphereColliderData;
					};
				}
			}
		}
	}
}