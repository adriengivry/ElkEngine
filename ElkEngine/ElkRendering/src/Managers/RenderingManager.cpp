#include "stdafxRendering.h"

#include <ElkTools/Debug/Log.h>
#include <ElkTools/Utils/Profiler.h>

#include "ElkRendering/Managers/RenderingManager.h"
#include "ElkRendering/Debug/GLDebug.h"

ElkRendering::Managers::RenderingManager::RenderingManager()
	: RenderingManager(800, 600)
{
}

ElkRendering::Managers::RenderingManager::RenderingManager(const uint16_t& p_windowWidth,
                                                           const uint16_t& p_windowHeight)
	: m_renderMode(RenderMode::NORMAL), m_width(p_windowWidth), m_height(p_windowHeight)
{
	ElkRendering::Managers::RenderingManager::Setup();
}

ElkRendering::Managers::RenderingManager::~RenderingManager() { ElkRendering::Managers::RenderingManager::Close(); }

void ElkRendering::Managers::RenderingManager::Setup()
{
	m_modelManager = std::make_unique<ElkRendering::Managers::ModelManager>(m_meshManager, m_textureManager);
	m_colliderMaterial.forceNoTexture = true;
	m_colliderMaterial.emissive = glm::vec3(1.0f, 1.0f, 0.0f);
	m_colliderMaterial.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	m_colliderMaterial.specular = glm::vec3(0.0f, 0.0f, 0.0f);
	m_colliderMaterial.ambient = glm::vec3(1.0f, 1.0f, 0.0f);
}

void ElkRendering::Managers::RenderingManager::Close()
{
}

void ElkRendering::Managers::RenderingManager::Draw(const Resources::Model& p_model, const Data::Material& p_material, const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale)
{
	ElkRendering::Resources::Shader* shader = m_shaderManager.RequireAndGet("standard");

	if (shader)
	{
		shader->Bind();
		UpdateShaderUniforms(p_model, p_material, *shader, p_position, p_rotation, p_scale);
		UpdateRenderMode();
		p_model.Draw(*shader);
		shader->Unbind();
	}
}

void ElkRendering::Managers::RenderingManager::DrawCollider(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale)
{
	Resources::Model* model = m_modelManager->RequireAndGet("Cube");

	if (model)
	{
		RenderMode previousMode = m_renderMode;
		m_renderMode = RenderMode::WIREFRAME;
		Draw(*model, m_colliderMaterial, p_position, p_rotation, p_scale);
		m_renderMode = previousMode;
	}
}

void ElkRendering::Managers::RenderingManager::UpdateRenderMode()
{
	GLenum renderMode;
	switch (m_renderMode)
	{
	case RenderMode::NORMAL:
		renderMode = GL_FILL;
		break;
	case RenderMode::WIREFRAME:
		renderMode = GL_LINE;
		break;
	default:
		renderMode = GL_FILL;
		break;
	}

	GLCall(glPolygonMode(GL_FRONT_AND_BACK, renderMode));
}

void ElkRendering::Managers::RenderingManager::UpdateShaderUniforms(const Resources::Model& p_model, const Data::Material& p_material, Resources::Shader& p_shader, const glm::vec3 & p_position, const glm::vec3 & p_rotation, const glm::vec3 & p_scale)
{
	PROFILER_SPY("RenderingManager::SetShaderUniforms");

	const float screenRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
	const glm::mat4 projection = glm::perspective(m_camera.GetZoom(), screenRatio, 0.1f, 100.0f);
	const glm::mat4 view = m_camera.GetViewMatrix();
	glm::mat4 model;

	model =	glm::translate(model, p_position);
	model =	glm::scale(model, p_scale);
	model =	model * glm::mat4(glm::quat(glm::radians(p_rotation)));

	p_shader.SetUniform("u_proj", projection);
	p_shader.SetUniform("u_view", view);
	p_shader.SetUniform("u_model", model);

	p_shader.SetUniform("u_viewPos", m_camera.GetPosition());

	p_shader.SetUniform("u_hasTexture", p_model.HasTextures());
	p_shader.SetUniform("u_material.color", p_material.color);
	p_shader.SetUniform("u_material.ambient", p_material.ambient);
	p_shader.SetUniform("u_material.diffuse", p_material.diffuse);
	p_shader.SetUniform("u_material.specular", p_material.specular);
	p_shader.SetUniform("u_material.shininess", p_material.shininess);
	p_shader.SetUniform("u_material.emissive", p_material.emissive);
	p_shader.SetUniform("u_material.forceNoTexture", p_material.forceNoTexture);

	p_shader.SetUniform("u_light.position", m_lightPosition);
	p_shader.SetUniform("u_light.ambient", m_light.ambient);
	p_shader.SetUniform("u_light.diffuse", m_light.diffuse);
	p_shader.SetUniform("u_light.specular", m_light.specular);
	p_shader.SetUniform("u_light.attenuation", m_light.attenuation);
	p_shader.SetUniform("u_light.color", m_light.color);
}

void ElkRendering::Managers::RenderingManager::SetCamera(const glm::vec3& p_position, const glm::vec3& p_rotation)
{
	m_camera.SetPosition(p_position);
	m_camera.ProcessRotationMovement(p_rotation);
}

void ElkRendering::Managers::RenderingManager::SetLight(const glm::vec3& p_position, const Data::Light& p_lightData)
{
	m_lightPosition = p_position;
	m_light.ambient = p_lightData.ambient;
	m_light.diffuse = p_lightData.diffuse;
	m_light.specular = p_lightData.specular;
	m_light.attenuation = p_lightData.attenuation;
	m_light.color = p_lightData.color;
}

void ElkRendering::Managers::RenderingManager::SetWindowSize(const uint16_t& p_width, const uint16_t& p_height)
{
	m_width = p_width;
	m_height = p_height;
}
