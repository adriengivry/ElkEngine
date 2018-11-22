#include "stdafx.h"

#include <ElkTools/Utils/IniLoader.h>
#include <ElkTools/Utils/Profiler.h>
#include <ElkTools/Utils/Time.h>

#include "ElkPhysics/Managers/PhysicsManager.h"

void ElkPhysics::Managers::PhysicsManager::Setup()
{
	SetGravity(ElkTools::Utils::IniLoader::GetFloat("physics.ini", "gravity"));
}

void ElkPhysics::Managers::PhysicsManager::Close()
{
}

void ElkPhysics::Managers::PhysicsManager::ClearPhysicsEntities()
{
	m_entities.clear();
}

void ElkPhysics::Managers::PhysicsManager::AddEntity(TransformData & p_transform, AColliderData & p_collider)
{
	m_entities.emplace_back(p_transform, p_collider);
}

void ElkPhysics::Managers::PhysicsManager::AddEntity(TransformData& p_transform, AColliderData& p_collider,
	RigidbodyData& p_rigidbody)
{
	m_entities.emplace_back(p_transform, p_collider, p_rigidbody);
}

void ElkPhysics::Managers::PhysicsManager::MoveEntites(double p_deltaTime)
{
	PROFILER_SPY("PhysicsManager::MoveEntities");

	for (Data::Entity::PhysicsEntity& entity : m_entities)
		if (entity.HasRigidbody())
			entity.GetTransform().localPosition += entity.GetRigidbody().velocity * static_cast<float>(p_deltaTime);
}

void ElkPhysics::Managers::PhysicsManager::SetGravity(float p_value)
{
	m_gravity = p_value;
}


void ElkPhysics::Managers::PhysicsManager::ApplyPhysics()
{
	PROFILER_SPY("PhysicsManager::ApplyPhysics");

	ApplyGravity(ElkTools::Utils::Time::GetDeltaTime());
	HandleCollisions(ElkTools::Utils::Time::GetDeltaTime());
	MoveEntites(ElkTools::Utils::Time::GetDeltaTime());
}

void ElkPhysics::Managers::PhysicsManager::ApplyGravity(double p_deltaTime)
{
	PROFILER_SPY("PhysicsManager::ApplyGravity");

	for (Data::Entity::PhysicsEntity& entity : m_entities)
		if (entity.HasRigidbody())
			entity.GetRigidbody().velocity += glm::vec3(0, -1, 0) * (m_gravity * static_cast<float>(p_deltaTime));
}

void ElkPhysics::Managers::PhysicsManager::HandleCollisions(double p_deltaTime)
{
	PROFILER_SPY("PhysicsManager::HandleCollisions");

	for (uint32_t i = 0; i < m_entities.size(); i++)
	{
		for (uint32_t j = 0; j < m_entities.size(); j++)
		{
			if (i != j)
			{
				CollisionData collision = CheckCollision(m_entities[i], m_entities[j], p_deltaTime);
				if (collision.colliding)
				{
					if (m_entities[i].HasRigidbody())
						m_entities[i].GetRigidbody().velocity = collision.direction;
				}
			}
		}
	}
}

uint16_t ElkPhysics::Managers::PhysicsManager::GetNumberOfEntities() const
{
	return static_cast<unsigned int>(m_entities.size());
}

CollisionData ElkPhysics::Managers::PhysicsManager::CheckCollision(Data::Entity::PhysicsEntity& p_firstEntity,
	Data::Entity::PhysicsEntity& p_secondEntity, double p_deltaTime)
{
	PROFILER_SPY("PhysicsManager::CheckCollision");

	glm::vec3 firstEntityNextPosition;
	glm::vec3 secondEntityNextPosition;
	if (p_firstEntity.HasRigidbody() && p_secondEntity.HasRigidbody())
	{
		firstEntityNextPosition = p_firstEntity.GetTransform().GetWorldPosition() + p_firstEntity.GetRigidbody().velocity * static_cast<float>(p_deltaTime);
		secondEntityNextPosition = p_secondEntity.GetTransform().GetWorldPosition() + p_secondEntity.GetRigidbody().velocity * static_cast<float>(p_deltaTime);
	}
	else if (p_firstEntity.HasRigidbody() && !p_secondEntity.HasRigidbody())
	{
		firstEntityNextPosition = p_firstEntity.GetTransform().GetWorldPosition() + p_firstEntity.GetRigidbody().velocity * static_cast<float>(p_deltaTime);
		secondEntityNextPosition = p_secondEntity.GetTransform().GetWorldPosition();
	}
	else if (!p_firstEntity.HasRigidbody() && p_secondEntity.HasRigidbody())
	{
		firstEntityNextPosition = p_firstEntity.GetTransform().GetWorldPosition();
		secondEntityNextPosition = p_secondEntity.GetTransform().GetWorldPosition() + p_secondEntity.GetRigidbody().velocity * static_cast<float>(p_deltaTime);
	}
	else if (!p_firstEntity.HasRigidbody() && !p_secondEntity.HasRigidbody())
	{
		return CollisionData(glm::vec3(), false);
	}
	if (p_firstEntity.GetCollider().type == AColliderData::ColliderType::BOX)
	{
		BoxColliderData& firstBox = static_cast<BoxColliderData&>(p_firstEntity.GetCollider());
		if (p_secondEntity.GetCollider().type == AColliderData::ColliderType::BOX)
		{
			BoxColliderData& secondBox = static_cast<BoxColliderData&>(p_secondEntity.GetCollider());
			glm::vec3 firstDistance = (secondEntityNextPosition - secondBox.extend) - (firstEntityNextPosition + firstBox.extend);
			glm::vec3 secondDistance = (firstEntityNextPosition - firstBox.extend) - (secondEntityNextPosition + secondBox.extend);

			glm::vec3 distance = glm::vec3(glm::max(firstDistance.x, secondDistance.x), glm::max(firstDistance.y, secondDistance.y), glm::max(firstDistance.z, secondDistance.z));
			float maxDistance = glm::max(glm::max(distance.x, distance.y), distance.z);
			if (maxDistance <= 0 && p_firstEntity.HasRigidbody())
			{
				
				glm::vec3 remainingDistance;
				glm::vec3 firstMinPoint(firstEntityNextPosition - firstBox.extend);
				glm::vec3 firstMaxPoint(firstEntityNextPosition + firstBox.extend);
				glm::vec3 secondMinPoints(secondEntityNextPosition - secondBox.extend);
				glm::vec3 secondMaxPoints(secondEntityNextPosition + secondBox.extend);

				if(secondMaxPoints.x - firstMinPoint.x >= -0.05f &&  secondMaxPoints.x - firstMinPoint.x <= 0.05f)
				{
					float remainingX = (p_secondEntity.GetTransform().GetWorldPosition().x + secondBox.extend.x) - (p_firstEntity.GetTransform().GetWorldPosition().x - firstBox.extend.x);
					remainingDistance = glm::vec3(remainingX / p_deltaTime, p_firstEntity.GetRigidbody().velocity.y, p_firstEntity.GetRigidbody().velocity.z);
				}
				else if(secondMinPoints.x - firstMaxPoint.x >= -0.05f &&  secondMinPoints.x - firstMaxPoint.x <= 0.05f)
				{
					float remainingX = (p_secondEntity.GetTransform().GetWorldPosition().x - secondBox.extend.x) - (p_firstEntity.GetTransform().GetWorldPosition().x + firstBox.extend.x);
					remainingDistance = glm::vec3(remainingX / p_deltaTime, p_firstEntity.GetRigidbody().velocity.y, p_firstEntity.GetRigidbody().velocity.z);
				}
				if (secondMaxPoints.y - firstMinPoint.y >= -0.05f &&  secondMaxPoints.y - firstMinPoint.y <= 0.05f)
				{
					float remainingY = (p_secondEntity.GetTransform().GetWorldPosition().y + secondBox.extend.y) - (p_firstEntity.GetTransform().GetWorldPosition().y - firstBox.extend.y);
					remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().velocity.x, remainingY / p_deltaTime, p_firstEntity.GetRigidbody().velocity.z);
				}
				else if (secondMinPoints.y - firstMaxPoint.y >= -0.05f &&  secondMinPoints.y - firstMaxPoint.y <= 0.05f)
				{
					float remainingY = (p_secondEntity.GetTransform().GetWorldPosition().y - secondBox.extend.y) - (p_firstEntity.GetTransform().GetWorldPosition().y + firstBox.extend.y);
					remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().velocity.x, remainingY / p_deltaTime, p_firstEntity.GetRigidbody().velocity.z);
				}
				if (secondMaxPoints.z - firstMinPoint.z >= -0.05f &&  secondMaxPoints.z - firstMinPoint.z <= 0.05f)
				{
					float remainingZ = (p_secondEntity.GetTransform().GetWorldPosition().z + secondBox.extend.z) - (p_firstEntity.GetTransform().GetWorldPosition().z - firstBox.extend.z);
					remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().velocity.x, p_firstEntity.GetRigidbody().velocity.y, remainingZ / p_deltaTime);
				}
				else if (secondMinPoints.z - firstMaxPoint.z >= -0.05f &&  secondMinPoints.z - firstMaxPoint.z <= 0.05f)
				{
					float remainingZ = (p_secondEntity.GetTransform().GetWorldPosition().z - secondBox.extend.z) - (p_firstEntity.GetTransform().GetWorldPosition().z + firstBox.extend.z);
					remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().velocity.x, p_firstEntity.GetRigidbody().velocity.y, remainingZ / p_deltaTime);
				}
				/*
				glm::vec3 distanceBetweenCenter = secondEntityNextPosition - firstEntityNextPosition;
				float biggest = glm::max(glm::max(glm::abs(distanceBetweenCenter.x), glm::abs(distanceBetweenCenter.y)), glm::abs(distanceBetweenCenter.z));
				if (biggest == glm::abs(distanceBetweenCenter.x))
				{
					if (distanceBetweenCenter.x < 0)
					{
						float remainingX = (p_secondEntity.GetTransform().GetWorldPosition().x + secondBox.GetExtend().x) - (p_firstEntity.GetTransform().GetWorldPosition().x - firstBox.GetExtend().x);
						remainingDistance = glm::vec3(remainingX / p_deltaTime, p_firstEntity.GetRigidbody().GetVelocity().y, p_firstEntity.GetRigidbody().GetVelocity().z);
					}
					else if (distanceBetweenCenter.x > 0)
					{
						float remainingX = (p_secondEntity.GetTransform().GetWorldPosition().x - secondBox.GetExtend().x) - (p_firstEntity.GetTransform().GetWorldPosition().x + firstBox.GetExtend().x);
						remainingDistance = glm::vec3(remainingX / p_deltaTime, p_firstEntity.GetRigidbody().GetVelocity().y, p_firstEntity.GetRigidbody().GetVelocity().z);
					}
				}
				else if (biggest == glm::abs(distanceBetweenCenter.y))
				{
					if (distanceBetweenCenter.y < 0)
					{
						float remainingY = (p_secondEntity.GetTransform().GetWorldPosition().y + secondBox.GetExtend().y) - (p_firstEntity.GetTransform().GetWorldPosition().y - firstBox.GetExtend().y);
						remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().GetVelocity().x, remainingY / p_deltaTime, p_firstEntity.GetRigidbody().GetVelocity().z);
					}
					else if (distanceBetweenCenter.y > 0)
					{
						float remainingY = (p_secondEntity.GetTransform().GetWorldPosition().y - secondBox.GetExtend().y) - (p_firstEntity.GetTransform().GetWorldPosition().y + firstBox.GetExtend().y);
						remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().GetVelocity().x, remainingY / p_deltaTime, p_firstEntity.GetRigidbody().GetVelocity().z);
					}
				}
				else
				{
					if (distanceBetweenCenter.z < 0)
					{
						float remainingZ = (p_secondEntity.GetTransform().GetWorldPosition().z + secondBox.GetExtend().z) - (p_firstEntity.GetTransform().GetWorldPosition().z - firstBox.GetExtend().z);
						remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().GetVelocity().x, p_firstEntity.GetRigidbody().GetVelocity().y, remainingZ / p_deltaTime);
					}
					else if (distanceBetweenCenter.z > 0)
					{
						float remainingZ = (p_secondEntity.GetTransform().GetWorldPosition().z - secondBox.GetExtend().z) - (p_firstEntity.GetTransform().GetWorldPosition().z + firstBox.GetExtend().z);
						remainingDistance = glm::vec3(p_firstEntity.GetRigidbody().GetVelocity().x, p_firstEntity.GetRigidbody().GetVelocity().y, remainingZ / p_deltaTime);
					}
				}
				*/

				return CollisionData(remainingDistance, true);
			}
			return CollisionData(distance, false);
		}

		SphereColliderData& secondSphere = static_cast<SphereColliderData&>(p_secondEntity.GetCollider());
		glm::vec3 direction = p_secondEntity.GetTransform().GetWorldPosition() - p_firstEntity.GetTransform().GetWorldPosition();
		glm::vec3 boxMinPoint = (p_firstEntity.GetTransform().GetWorldPosition() - firstBox.extend);
		glm::vec3 boxMaxPoint = (p_firstEntity.GetTransform().GetWorldPosition() + firstBox.extend);
		float distance = 0;
		if (p_secondEntity.GetTransform().GetWorldPosition().x < boxMinPoint.x)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().x - boxMinPoint.x), 2);
		}
		else if (p_secondEntity.GetTransform().GetWorldPosition().x >= boxMaxPoint.x)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().x - boxMaxPoint.x), 2);
		}
		if (p_secondEntity.GetTransform().GetWorldPosition().y < boxMinPoint.y)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().y - boxMinPoint.y), 2);
		}
		else if (p_secondEntity.GetTransform().GetWorldPosition().y > boxMaxPoint.y)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().y - boxMaxPoint.y), 2);
		}
		if (p_secondEntity.GetTransform().GetWorldPosition().z < boxMinPoint.z)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().z - boxMinPoint.z), 2);
		}
		else if (p_secondEntity.GetTransform().GetWorldPosition().z > boxMaxPoint.z)
		{
			distance += glm::pow((p_secondEntity.GetTransform().GetWorldPosition().z - boxMaxPoint.z), 2);
		}
		return CollisionData(direction, distance <= glm::pow(secondSphere.radius, 2));
	}
	SphereColliderData& firstSphere = static_cast<SphereColliderData&>(p_firstEntity.GetCollider());
	if (p_secondEntity.GetCollider().type == AColliderData::ColliderType::BOX)
	{
		BoxColliderData& secondBox = static_cast<BoxColliderData&>(p_secondEntity.GetCollider());
		glm::vec3 direction = p_firstEntity.GetTransform().GetWorldPosition() - p_secondEntity.GetTransform().GetWorldPosition();
		glm::vec3 boxMinPoint = (p_secondEntity.GetTransform().GetWorldPosition() - secondBox.extend);
		glm::vec3 boxMaxPoint = (p_secondEntity.GetTransform().GetWorldPosition() + secondBox.extend);
		float distance = 0;
		if (p_firstEntity.GetTransform().GetWorldPosition().x < boxMinPoint.x)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().x - boxMinPoint.x), 2);
		}
		else if (p_firstEntity.GetTransform().GetWorldPosition().x >= boxMaxPoint.x)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().x - boxMaxPoint.x), 2);
		}
		if (p_firstEntity.GetTransform().GetWorldPosition().y < boxMinPoint.y)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().y - boxMinPoint.y), 2);
		}
		else if (p_firstEntity.GetTransform().GetWorldPosition().y > boxMaxPoint.y)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().y - boxMaxPoint.y), 2);
		}
		if (p_firstEntity.GetTransform().GetWorldPosition().z < boxMinPoint.z)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().z - boxMinPoint.z), 2);
		}
		else if (p_firstEntity.GetTransform().GetWorldPosition().z > boxMaxPoint.z)
		{
			distance += glm::pow((p_firstEntity.GetTransform().GetWorldPosition().z - boxMaxPoint.z), 2);
		}
		return CollisionData(direction, distance <= glm::pow(firstSphere.radius, 2));
	}

	SphereColliderData& secondSphere = static_cast<SphereColliderData&>(p_secondEntity.GetCollider());
	glm::vec3 direction = p_secondEntity.GetTransform().GetWorldPosition() - p_firstEntity.GetTransform().GetWorldPosition();
	float distanceBetweenCenters = glm::length(direction);
	float radiusDistance = firstSphere.radius + secondSphere.radius;
	direction /= distanceBetweenCenters;

	float distance = distanceBetweenCenters - radiusDistance;
	return CollisionData(direction * distance, distance <= 0);
}