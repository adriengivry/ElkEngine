#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Math/Math.h"
#include "ElkPhysics/Data/Colliders/AColliderData.h"
#include "ElkPhysics/Data/Colliders/BoxColliderData.h"

using namespace ElkPhysics::Data::Colliders;
namespace ElkPhysics
{
	namespace Data
	{
		namespace Colliders
		{
			struct ELK_PHYSICS_EXPORT SphereColliderData : public AColliderData
			{
				SphereColliderData();
				~SphereColliderData() = default;

				float radius;
			};
		}
	}
}