#pragma once

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Data/Physics/TransformData.h"
#include "ElkPhysics/Data/Physics/RigidbodyData.h"

namespace ElkPhysics
{
	namespace Data
	{
		namespace Entity
		{
			class ELK_PHYSICS_EXPORT PhysicsEntity
			{
			public:
				PhysicsEntity(Physics::TransformData& p_transform, Colliders::AColliderData& p_collider);
				PhysicsEntity(Physics::TransformData& p_transform, Colliders::AColliderData& p_collider, Physics::RigidbodyData& p_rigidBody);
				~PhysicsEntity() = default;

				Physics::TransformData& GetTransform();
				Colliders::AColliderData& GetCollider();
				Physics::RigidbodyData& GetRigidbody();

				bool HasRigidbody() { return m_rigidbody != nullptr; }

			private:
				Physics::TransformData* m_transform;
				Colliders::AColliderData* m_collider;
				Physics::RigidbodyData* m_rigidbody;
			};
		}
	}
}