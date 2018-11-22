#include "stdafx.h"

#include "ElkPhysics/Data/Entity/PhysicsEntity.h"

#include <iostream>

ElkPhysics::Data::Entity::PhysicsEntity::PhysicsEntity(Physics::TransformData & p_transform, Colliders::AColliderData & p_collider)
	: m_transform(&p_transform), m_collider(&p_collider), m_rigidbody(nullptr)
{
}

ElkPhysics::Data::Entity::PhysicsEntity::PhysicsEntity(Physics::TransformData & p_transform, Colliders::AColliderData & p_collider, Physics::RigidbodyData & p_rigidbody)
	: m_transform(&p_transform), m_collider(&p_collider), m_rigidbody(&p_rigidbody)
{
}

ElkPhysics::Data::Physics::RigidbodyData& ElkPhysics::Data::Entity::PhysicsEntity::GetRigidbody()
{
	return *m_rigidbody;
}

ElkPhysics::Data::Physics::TransformData& ElkPhysics::Data::Entity::PhysicsEntity::GetTransform()
{
	return *m_transform;
}

ElkPhysics::Data::Colliders::AColliderData & ElkPhysics::Data::Entity::PhysicsEntity::GetCollider()
{
	return *m_collider;
}
