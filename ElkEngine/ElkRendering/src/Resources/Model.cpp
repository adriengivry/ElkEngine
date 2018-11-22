#include "stdafxRendering.h"

#include <ElkTools/Utils/Profiler.h>

#include "ElkRendering/Resources/Model.h"
#include "ElkRendering/Managers/TextureManager.h"

using namespace ElkRendering::Resources;
using namespace ElkRendering::Managers;

Model::Model(MeshManager& p_meshManager,
             TextureManager& p_textureManager)
	: m_meshManager(p_meshManager),
	  m_textureManager(p_textureManager)
{
}

/*----- Constructot Destructor Copy -----*/
Model::Model(const std::string& p_filePath,
             MeshManager& p_meshManager,
             TextureManager& p_textureManager)
	: Model(p_meshManager, p_textureManager)
{
	LoadFromFile(p_filePath);
}

Model::Model(const Model& p_other)
	: m_meshManager(p_other.m_meshManager),
	  m_textureManager(p_other.m_textureManager),
	  m_meshes(p_other.m_meshes),
	  m_directory(p_other.m_directory)
{
}

Model::Model(Model&& p_other) noexcept
	: m_meshManager(p_other.m_meshManager),
	  m_textureManager(p_other.m_textureManager),
	  m_meshes(std::move(p_other.m_meshes)),
	  m_directory(std::move(p_other.m_directory))
{
}

Model& Model::operator=(const Model& p_other)
{
	if (this == &p_other)
		return *this;
	m_meshes = p_other.m_meshes;
	m_directory = p_other.m_directory;
	return *this;
}

Model& Model::operator=(Model&& p_other) noexcept
{
	if (this == &p_other)
		return *this;
	m_meshes = std::move(p_other.m_meshes);
	m_directory = std::move(p_other.m_directory);
	return *this;
}

/*--------------------------------------*/

bool ElkRendering::Resources::Model::HasTextures() const
{
	for (Mesh* mesh : m_meshes)
	{
		if (mesh->HasTexutres())
			return true;
	}

	return false;
}

void Model::Draw(Shader& p_shader) const noexcept
{
	PROFILER_SPY("Model::Draw");

	for (Mesh* mesh : m_meshes)
	{
		mesh->Bind();
		mesh->Draw(p_shader);
		mesh->Unbind();
	}
}

bool Model::LoadFromFile(const std::string& p_path)
{
	return LoadModel(p_path);
}

bool Model::LoadModel(const std::string& p_filePath) noexcept
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(p_filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ElkTools::Debug::Log::Process(std::string("ERROR::ASSIMP::") + import.GetErrorString(), ElkTools::Debug::Log::LogLevel::LOG_ERROR);
		return false;
	}
	m_directory = p_filePath.substr(0, p_filePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	return true;
}

/*----- Private Functions -----*/
void Model::ProcessNode(aiNode* p_node, const aiScene* p_scene) noexcept
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
	{
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		Mesh* meshObj = ProcessMesh(mesh, p_scene);
		meshObj->Unbind();
		m_meshes.push_back(meshObj);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < p_node->mNumChildren; i++)
	{
		ProcessNode(p_node->mChildren[i], p_scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* p_mesh, const aiScene* p_scene) const noexcept
{
	std::vector<Data::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
	{
		Data::Vertex vertex{};

		// process vertex positions, normals and texture coordinates

		// Positions
		vertex.position.x = p_mesh->mVertices[i].x;
		vertex.position.y = p_mesh->mVertices[i].y;
		vertex.position.z = p_mesh->mVertices[i].z;

		// Normals
		if (p_mesh->mNormals)
		{
			vertex.normal.x = p_mesh->mNormals[i].x;
			vertex.normal.y = p_mesh->mNormals[i].y;
			vertex.normal.z = p_mesh->mNormals[i].z;
		}
		else
		{
			vertex.normal.x = 0;
			vertex.normal.y = 0;
			vertex.normal.z = 0;
		}

		// Text coords
		if (p_mesh->mTextureCoords[0])
		{
			vertex.textureCoord.x = p_mesh->mTextureCoords[0][i].x;
			vertex.textureCoord.y = p_mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.textureCoord.x = 0.0f;
			vertex.textureCoord.y = 0.0f;
		}

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
	{
		const aiFace face = p_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (p_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. normal maps
		std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return m_meshManager.AddAndGetMesh(new Mesh(vertices, indices, textures));
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* p_mat,
                                                  const aiTextureType p_type,
                                                  const std::string p_typeName) const noexcept
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++)
	{
		aiString nameWhitExtention;
		p_mat->GetTexture(p_type, i, &nameWhitExtention);

		textures.emplace_back(m_textureManager.RequireAndGet(
									std::string(nameWhitExtention.C_Str())));
		textures.back()->SetType(p_typeName);
	}

	return textures;
}

/*-----------------------------*/
