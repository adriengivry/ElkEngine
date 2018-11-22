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
				class ELK_GAMEENGINE_EXPORT ModelRenderer : public ABehaviour
				{
				public:
					ModelRenderer();
					~ModelRenderer() = default;

					ElkRendering::Resources::Model& GetModel();
					ElkRendering::Data::Material& GetMaterial();

					void SetModel(ElkRendering::Resources::Model* p_newModel);

					bool HasModel() const;

				private:
					ElkRendering::Resources::Model* m_model;
					ElkRendering::Data::Material m_material;
				};
			}
		}
	}
}
