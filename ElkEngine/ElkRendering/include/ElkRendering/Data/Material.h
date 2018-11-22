#pragma once

#include "Color.h"

namespace ElkRendering
{
	namespace Data
	{
		struct Material : public ElkAPI::IDataStructure
		{
			glm::vec3 color;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			glm::vec3 emissive;
			float shininess;
			bool forceNoTexture;

			Material() { Material::Setup(); }

			void Setup() override
			{
				color = glm::vec3(1.0f, 1.0f, 1.0f);
				ambient = glm::vec3(0.5f, 0.5f, 0.5f);
				diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
				specular = glm::vec3(0.5f, 0.5f, 0.5f);
				emissive = glm::vec3(0.0f, 0.0f, 0.0f);
				shininess = 32.0f;
				forceNoTexture = false;
			}
		};
	}
}
