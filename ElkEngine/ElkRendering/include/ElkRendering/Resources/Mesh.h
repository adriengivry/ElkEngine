#pragma once

#include "ElkRendering/Data/Vertex.h"
#include "ElkRendering/Resources/Texture.h"
#include "ElkRendering/Resources/Shader.h"

namespace ElkRendering
{
	namespace Resources
	{
		/*
		* A resource that can be given to a MeshRenderer in order to get displayed
		*/
		class Mesh
		{
		private:
			// OPENGL BUFFERS
			unsigned m_vbo;
			unsigned m_vao;
			unsigned m_ebo;

			// MESH DATA
			std::vector<Data::Vertex> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<Texture*> m_textures;

		public:
			Mesh() = default;
			explicit Mesh(std::vector<Data::Vertex> p_vertices,
			              std::vector<unsigned int> p_indices,
			              std::vector<Texture*> p_textures);
			~Mesh() = default;

			void Setup();
			void Draw(Shader& p_shader);

			bool HasTexutres() const;

			void Bind() const;
			void Unbind() const;
			std::vector<unsigned int>& GetIndices();
		};
	}
}
