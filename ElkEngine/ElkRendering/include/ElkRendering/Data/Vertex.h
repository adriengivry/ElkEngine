#pragma once

#include <ElkAPI\IDataStructure.h>

namespace ElkRendering
{
	namespace Data
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoord;
		};
	}
}
