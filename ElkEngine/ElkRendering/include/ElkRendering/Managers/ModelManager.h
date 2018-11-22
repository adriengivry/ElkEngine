#pragma once

#include <ElkTools/Managers/AResourceManager.h>

#include "ElkRendering/exportRendering.h"
#include "ElkRendering/Resources/Model.h"

namespace ElkRendering
{
	namespace Managers
	{
		class ELK_RENDERING_EXPORT ModelManager : public ElkTools::Managers::AResourceManager<Resources::Model>
		{
		private :
			Managers::MeshManager& m_meshManager;
			Managers::TextureManager& m_textureManager;

		public:
			ModelManager(MeshManager& p_meshManager,
			             TextureManager& p_textureManager)
				: AResourceManager(ElkTools::Utils::SharedData::__RESOURCE_MANAGERS_FOLDER_PATH + "models.cfg"),
				  m_meshManager(p_meshManager),
				  m_textureManager(p_textureManager)
			{
			}

			Resources::Model* Load(const std::string& p_path) override
			{
				Resources::Model* model = new Resources::Model(m_meshManager, m_textureManager);
				if (!model->LoadFromFile(p_path))
				{
					delete model;
					model = nullptr;
					ElkTools::Debug::Log::Process("Failed to load model: " + p_path, ElkTools::Debug::Log::LogLevel::LOG_ERROR);
				}

				return model;
			}
		};
	}
}
