#pragma once

#include <ElkAPI\IManager.h>

#include "ElkRendering/exportRendering.h"
#include "ElkRendering/Managers/ModelManager.h"
#include "ElkRendering/Managers/MeshManager.h"
#include "ElkRendering/Managers/TextureManager.h"
#include "ElkRendering/Managers/ShaderManager.h"
#include "ElkRendering/Data/CameraData.h"
#include "ElkRendering/Data/Material.h"
#include "ElkRendering/Data/Light.h"

namespace ElkRendering
{
	namespace Resources
	{
		class Model;
	}
}

namespace ElkRendering
{
	namespace Managers
	{
		/*
		* The renderer is pretty usefull to print things on the screen !
		*/
		class ELK_RENDERING_EXPORT RenderingManager : public ElkAPI::IManager
		{
		public:
			enum class RenderMode
			{
				NORMAL,
				WIREFRAME
			};

		private:
			RenderMode m_renderMode;

			std::unique_ptr<ModelManager> m_modelManager;
			MeshManager m_meshManager;
			TextureManager m_textureManager;
			ShaderManager m_shaderManager;
			Data::CameraData m_camera;

			Data::Light m_light;
			glm::vec3 m_lightPosition;

			Data::Material m_colliderMaterial;

			uint16_t m_width;
			uint16_t m_height;

		public:
			RenderingManager();
			RenderingManager(const uint16_t& p_windowWidth, const uint16_t& p_windowHeight);
			~RenderingManager();

			void Setup() override;
			void Close() override;

			ModelManager& GetModelManager() { return *m_modelManager; }
			TextureManager& GetTextureManager() { return m_textureManager; }
			ShaderManager& GetShaderManager() { return m_shaderManager; }

			void SetCamera(const glm::vec3& p_position, const glm::vec3& p_rotation);
			void SetLight(const glm::vec3& p_position, const Data::Light& p_lightData);
			void SetWindowSize(const uint16_t& p_width, const uint16_t& p_height);
			void SetRenderMode(const RenderMode& p_renderMode) { m_renderMode = p_renderMode; };

			void Draw(const Resources::Model& p_model, const Data::Material& p_material, const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);
			void DrawCollider(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);

		private:
			void UpdateShaderUniforms(const Resources::Model&, const Data::Material& p_material, Resources::Shader& p_shader, const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);
			void UpdateRenderMode();
		};
	}
}
