#pragma once

#include <ElkPhysics/Data/Colliders/AColliderData.h>

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
					class ELK_GAMEENGINE_EXPORT ACollider : public Objects::Components::Behaviours::ABehaviour
					{
					public:
						virtual ElkPhysics::Data::Colliders::AColliderData* GetColliderData() = 0;
					};
				}
			}
		}
	}
}
