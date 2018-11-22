#pragma once

#include "ElkRendering/Resources/Mesh.h"

namespace ElkRendering
{
	namespace Managers
	{
		/*
		 * Manager that can handle Mesh
		 */
		class MeshManager
		{
		private:
			std::vector<Resources::Mesh*> m_meshes;

		public:
			MeshManager() = default;
			~MeshManager();

			void AddMesh(Resources::Mesh* p_mesh);
			Resources::Mesh* AddAndGetMesh(Resources::Mesh* p_mesh);
			Resources::Mesh* GetMesh(const unsigned p_id) const;

			void PurgeMeshes();
		};
	}
}