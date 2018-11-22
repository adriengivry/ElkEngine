#pragma once

#include <ElkRendering\Data\Light.h>

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
				/*
				* Component to set light
				*/
				class ELK_GAMEENGINE_EXPORT Light : public ABehaviour
				{
				public:
					Light();
					~Light() = default;

					const glm::vec3& GetAmbient() { return m_lightData.ambient; }
					const glm::vec3& GetDiffuse() { return m_lightData.diffuse; }
					const glm::vec3& GetSpecular() { return m_lightData.specular; }
					const glm::vec3& GetColor() { return m_lightData.color; }
					const float GetAttenuation() { return m_lightData.attenuation; }

					void SetAmbient(const glm::vec3& p_value) { m_lightData.ambient = p_value; }
					void SetDiffuse(const glm::vec3& p_value) { m_lightData.diffuse = p_value; }
					void SetSpecular(const glm::vec3& p_value) { m_lightData.specular = p_value; }
					void SetColor(const glm::vec3& p_value) { m_lightData.color = p_value; }
					void SetAttenuation(const float& p_value) { m_lightData.attenuation = p_value; }

					ElkRendering::Data::Light& GetLightData() { return m_lightData; }

				private:
					ElkRendering::Data::Light m_lightData;
				};
			}
		}
	}
}