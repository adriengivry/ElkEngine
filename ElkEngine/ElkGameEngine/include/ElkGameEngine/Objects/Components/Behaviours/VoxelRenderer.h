#pragma once

#include <ElkRendering/Resources/Model.h>
#include <ElkRendering/Data/Material.h>

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
				* 3D Model of an object
				*/
				class ELK_GAMEENGINE_EXPORT VoxelRenderer : public ABehaviour
				{
				public:
					VoxelRenderer();
					~VoxelRenderer() = default;
				};
			}
		}
	}
}
