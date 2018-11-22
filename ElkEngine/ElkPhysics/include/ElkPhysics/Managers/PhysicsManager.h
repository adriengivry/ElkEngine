#pragma once

#include <vector>
#include <ElkAPI\IManager.h>

#include "ElkPhysics/exportPhysics.h"
#include "ElkPhysics/Data/Colliders/AColliderData.h"
#include "ElkPhysics/Data/Entity/PhysicsEntity.h"
#include "ElkPhysics/Data/Colliders/CollisionData.h"
#include "ElkPhysics/Data/Colliders/BoxColliderData.h"
#include "ElkPhysics/Data/Colliders/SphereColliderData.h"

using namespace ElkPhysics::Data::Colliders;
using namespace ElkPhysics::Data::Physics;

namespace ElkPhysics
{
	namespace Managers
	{
		class ELK_PHYSICS_EXPORT PhysicsManager : public ElkAPI::IManager
		{
		public:
			PhysicsManager() { Setup(); }
			~PhysicsManager() { Close(); }

			void Setup();
			void Close();

			void ClearPhysicsEntities();
			void AddEntity(TransformData& p_transform, AColliderData& p_collider);
			void AddEntity(TransformData& p_transform, AColliderData& p_collider, RigidbodyData& p_rigidbody);

			void ApplyPhysics();
			void ApplyGravity(double p_deltaTime);
			void HandleCollisions(double p_deltaTime);
			void MoveEntites(double p_deltaTime);

			void SetGravity(float p_value);

			uint16_t GetNumberOfEntities() const;

			CollisionData CheckCollision(Data::Entity::PhysicsEntity& p_firstEntity, Data::Entity::PhysicsEntity& p_secondEntity, double p_deltaTime);

		private:
			std::vector<Data::Entity::PhysicsEntity> m_entities;
			float m_gravity;
		};
	}
}