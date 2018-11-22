#pragma once

#include <ElkAPI/IDataStructure.h>

namespace ElkRendering
{
	namespace Data
	{
		struct Light : public ElkAPI::IDataStructure
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			glm::vec3 color;
			float attenuation;

			Light() { Light::Setup(); }

			void Setup() override
			{
				ambient			= glm::vec3(0.0f, 0.0f, 0.0f);
				diffuse			= glm::vec3(1.0f, 1.0f, 1.0f);
				specular		= glm::vec3(1.0f, 1.0f, 1.0f);
				color			= glm::vec3(1.0f, 1.0f, 1.0f);
				attenuation		= 0.2f;
			}

		};
	}
}
