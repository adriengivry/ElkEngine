#include "stdafxRendering.h"

#include <ElkTools/Utils/Profiler.h>

#include "ElkRendering/Resources/Voxel.h"
#include "ElkRendering/Debug/GLDebug.h"

using namespace ElkRendering::Resources;
using namespace ElkRendering::Data;

Voxel::Voxel()
{
	Setup();
}

void Voxel::Setup()
{
	GLfloat verts[] =
	{
		0.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Voxel::Draw(Shader& p_shader)
{
	PROFILER_SPY("Voxel::Draw");

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

bool ElkRendering::Resources::Voxel::HasTexutres() const
{
	return m_textures.size() > 0;
}

void Voxel::Bind() const
{

}

void Voxel::Unbind() const
{

}

std::vector<unsigned>& Voxel::GetIndices()
{
	return m_indices;
}
