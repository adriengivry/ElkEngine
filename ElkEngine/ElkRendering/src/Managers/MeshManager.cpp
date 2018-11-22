#include "stdafxRendering.h"

#include <ElkTools\Debug\Log.h>

#include "ElkRendering/Managers/MeshManager.h"

using namespace ElkRendering::Resources;
using namespace ElkRendering::Managers;

MeshManager::~MeshManager()
{
	PurgeMeshes();
}

void MeshManager::AddMesh(Mesh* p_mesh)
{
	for (unsigned i = 0; i < m_meshes.size(); ++i)
	{
		if (m_meshes[i] == p_mesh)
		{
			ElkTools::Debug::Log::Process("Can't add a mesh a mesh already existing", ElkTools::Debug::Log::LogLevel::LOG_WARNING);
			return;
		}
	}

	m_meshes.emplace_back(p_mesh);
}

Mesh* MeshManager::AddAndGetMesh(Mesh* p_mesh)
{
	AddMesh(p_mesh);
	return p_mesh;
}

Mesh* MeshManager::GetMesh(const unsigned p_id) const
{
	return p_id < m_meshes.size() ? m_meshes[p_id] : nullptr;
}

void MeshManager::PurgeMeshes()
{
	for (Mesh* mesh : m_meshes)
	{
		delete mesh;
	}
}

