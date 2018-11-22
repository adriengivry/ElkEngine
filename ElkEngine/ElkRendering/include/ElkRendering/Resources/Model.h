#pragma once

#include "ElkRendering/exportRendering.h"
#include "ElkRendering/Managers/MeshManager.h"
#include "ElkRendering/Managers/TextureManager.h"
#include "ElkRendering/Resources/Shader.h"

namespace ElkRendering
{
	namespace Resources
	{
		class ELK_RENDERING_EXPORT Model
		{
		private:
			Managers::MeshManager& m_meshManager;
			Managers::TextureManager& m_textureManager;

			std::vector<Mesh*> m_meshes{};
			std::string m_directory;

		public:
			/*----- Constructor Destructor Copy -----*/
			explicit Model(Managers::MeshManager& p_meshManager,
			               Managers::TextureManager& p_textureManager);

			explicit Model(const std::string& p_filePath,
			               Managers::MeshManager& p_meshManager,
			               Managers::TextureManager& p_textureManager);

			Model(const Model& p_other);
			Model(Model&& p_other) noexcept;

			~Model() = default;

			Model& operator=(const Model& p_other);
			Model& operator=(Model&& p_other) noexcept;
			/*--------------------------------------*/

			bool HasTextures() const;

			void Draw(Shader& p_shader) const noexcept;
			bool LoadFromFile(const std::string& p_path);

		private:
			/*----- Private Functions -----*/
			bool LoadModel(const std::string& p_filePath) noexcept;
			void ProcessNode(aiNode* p_node, const aiScene* p_scene) noexcept;
			Mesh* ProcessMesh(aiMesh* p_mesh, const aiScene* p_scene) const noexcept;
			std::vector<Texture*> LoadMaterialTextures(aiMaterial* p_mat,
			                                           aiTextureType p_type,
			                                           std::string p_typeName) const noexcept;
			/*---------------------------------------*/
		};
	}
}
