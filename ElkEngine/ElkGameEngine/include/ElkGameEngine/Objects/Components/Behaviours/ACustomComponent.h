#pragma once

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ABehaviour.h"
#include "ElkGameEngine/Objects/GameObject.h"

namespace ElkGameEngine
{
	namespace Objects
	{
		namespace Components
		{
			namespace Behaviours
			{
				class ELK_GAMEENGINE_EXPORT ACustomComponent : public ABehaviour
				{
				public:
					ACustomComponent() = default;
					~ACustomComponent() = default;

					virtual void Update() = 0;
					virtual void OnAwake() = 0;
					virtual void OnDeath() = 0;
				};
			}
		}
	}
}
