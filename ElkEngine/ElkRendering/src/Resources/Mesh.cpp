#include "stdafxRendering.h"

#include <ElkTools/Utils/Profiler.h>

#include "ElkRendering/Resources/Mesh.h"
#include "ElkRendering/Debug/GLDebug.h"

using namespace ElkRendering::Resources;
using namespace ElkRendering::Data;

Mesh::Mesh(std::vector<Vertex> p_vertices,
           std::vector<unsigned> p_indices,
           std::vector<Texture*> p_textures)
	: m_vbo(0), m_vao(0), m_ebo(0),
	  m_vertices(std::move(p_vertices)),
	  m_indices(std::move(p_indices)),
	  m_textures(std::move(p_textures))
{
	Setup();
}

void Mesh::Setup()
{
	// create buffers/arrays
	GLCall(glGenVertexArrays(1, &m_vao));
	GLCall(glGenBuffers(1, &m_vbo));
	GLCall(glGenBuffers(1, &m_ebo));

	GLCall(glBindVertexArray(m_vao));

	// load data into vertex buffers
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW));

	// set the vertex attribute pointers
	// vertex Positions
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr));

	// vertex normals
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal))));

	// vertex texture coords
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureCoord))));

	GLCall(glBindVertexArray(0));
}

void Mesh::Draw(Shader& p_shader)
{
	PROFILER_SPY("Mesh::Draw");

	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		const std::string name = m_textures[i]->GetType();

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		p_shader.SetUniform(name + number, static_cast<int>(i));
		// and finally bind the texture

		m_textures[i]->Bind(i);

	}
	p_shader.Bind();

	GLCall(glBindVertexArray(m_vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	// draw mesh
	GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0));

	// always good practice to set everything back to defaults once configured.
	GLCall(glActiveTexture(GL_TEXTURE0));
	Texture::Unbind();
}

bool ElkRendering::Resources::Mesh::HasTexutres() const
{
	return m_textures.size() > 0;
}

void Mesh::Bind() const
{
	GLCall(glBindVertexArray(m_vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
}

void Mesh::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

std::vector<unsigned>& Mesh::GetIndices()
{
	return m_indices;
}
