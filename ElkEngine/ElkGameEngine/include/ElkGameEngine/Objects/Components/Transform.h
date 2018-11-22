#pragma once

#include <ElkPhysics/Data/Physics/TransformData.h>

#include "ElkGameEngine/export.h"
#include "ElkGameEngine/Objects/Components/AComponent.h"

namespace ElkGameEngine
{
	namespace Objects
	{
		namespace Components
		{
			/*
			* Position, rotation and scale of any GameObject
			*/
			class ELK_GAMEENGINE_EXPORT Transform : public AComponent
			{
			public:
				Transform();
				explicit Transform(const glm::vec3& p_position);
				explicit Transform(const glm::vec3& p_position, const glm::vec3& p_rotation);
				explicit Transform(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);
				~Transform() = default;

				glm::vec3 GetWorldPosition();
				glm::vec3 GetWorldRotation();
				glm::vec3 GetWorldScale();

				glm::vec3& GetLocalPosition();
				glm::vec3& GetLocalRotation();
				glm::vec3& GetLocalScale();

				glm::vec3& GetPosition();
				glm::vec3& GetRotation();
				glm::vec3& GetScale();

				void SetPosition(const glm::vec3& p_position);
				void SetRotation(const glm::vec3& p_rotation);
				void SetScale(const glm::vec3& p_scale);

				void SetParent(Transform& p_parent);

				void Translate(const glm::vec3& p_translate);
				void Rotate(const glm::vec3& p_rotate);
				void Scale(const glm::vec3& p_scale);

				ElkPhysics::Data::Physics::TransformData& GetTransformData() { return m_transformData; }

			private:
				ElkPhysics::Data::Physics::TransformData m_transformData;
			};
		}
	}
}
