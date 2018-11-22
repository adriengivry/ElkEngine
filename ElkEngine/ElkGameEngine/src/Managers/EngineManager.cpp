#include "stdafxGameEngine.h"

#include <ElkTools/Debug/Log.h>
#include <ElkTools/Utils/Time.h>

#include "ElkGameEngine/Managers/EngineManager.h"

using namespace ElkGameEngine::Objects::Components;
using namespace ElkGameEngine::Objects::Components::Behaviours;
using namespace ElkGameEngine::Objects::Components::Behaviours::Physics;

void ElkGameEngine::Managers::EngineManager::Close() {}

void ElkGameEngine::Managers::EngineManager::Quit()
{
	m_quit = true;
}

bool ElkGameEngine::Managers::EngineManager::IsRunning() const
{
	return m_quit ? false : m_windowManager->GetDriver()->IsActive() && m_windowManager->GetDevice()->IsActive();
}

void ElkGameEngine::Managers::EngineManager::PreUpdate()
{
	PROFILER_SPY("EngineManager::PreUpdate");

	m_time.Update();
	m_benchmark.Update();

	if (m_showFramerate)
		ShowFramerate();
	
	if (m_enableParsing)
		ParseScene();

	if (m_enablePhysics)
		UpdatePhysics();
}

void ElkGameEngine::Managers::EngineManager::PostUpdate()
{
	{
		PROFILER_SPY("EngineManager::PostUpdate");

		if (m_enableRendering)
			HandleRendering();

		HandleInputs();
	}

	m_profiler.Log();
}

void ElkGameEngine::Managers::EngineManager::UpdatePhysics()
{
	PROFILER_SPY("EngineManager::UpdatePhysics");

	for (auto& entity : m_toCollideWith)
	{
		Transform& transform = entity.first;
		BoxCollider& boxCollider = entity.second;
		m_physicsManager->AddEntity(transform.GetTransformData(), *boxCollider.GetColliderData());
	}

	for (auto& entity : m_toApplyPhysicOn)
	{
		Transform& transform = entity.first;
		BoxCollider& boxCollider = entity.second.first;
		Rigidbody& rigidbody = entity.second.second;
		m_physicsManager->AddEntity(transform.GetTransformData(), *boxCollider.GetColliderData(), *rigidbody.GetRigidbodyData());
	}

	m_physicsManager->ApplyPhysics();
	m_physicsManager->ClearPhysicsEntities();
}

void ElkGameEngine::Managers::EngineManager::RenderScene()
{
	if (m_enableModelRendering)
	{
		for (auto& entity : m_toDraw)
		{
			Transform& transform = entity.first;
			ModelRenderer& modelRenderer = entity.second;
			m_renderingManager->Draw(modelRenderer.GetModel(), modelRenderer.GetMaterial(), transform.GetPosition(), transform.GetRotation(), transform.GetScale());
		}
	}

	if (m_enableColliderRendering)
	{
		for (auto& entity : m_toDrawCollider)
		{
			Transform& transform = entity.first;
			BoxCollider& boxCollider = entity.second;
			m_renderingManager->DrawCollider(transform.GetPosition(), transform.GetRotation(), boxCollider.GetExtend() * 2.0f);
		}
	}
}

void ElkGameEngine::Managers::EngineManager::ParseScene()
{
	PROFILER_SPY("EngineManager::ParseScene");

	m_toDraw.clear();
	m_toDrawCollider.clear();
	m_toApplyPhysicOn.clear();
	m_toCollideWith.clear();
	ParseSceneRecursive(m_sceneManager->GetCurrentScene().GetRoot());
}

void ElkGameEngine::Managers::EngineManager::ParseSceneRecursive(Objects::GameObject & p_entity)
{
	auto transform = p_entity.GetComponent<Transform>();
	auto modelRenderer = p_entity.GetComponent<ModelRenderer>();
	auto camera = p_entity.GetComponent<Camera>();
	auto light = p_entity.GetComponent<Light>();
	auto boxCollider = p_entity.GetComponent<BoxCollider>();
	auto rigidbody = p_entity.GetComponent<Rigidbody>();

	if (modelRenderer)
		m_toDraw.push_back(std::pair<Transform&, ModelRenderer&>(*transform, *modelRenderer));

	if (boxCollider)
		m_toDrawCollider.push_back(std::pair<Transform&, BoxCollider&>(*transform, *boxCollider));

	if (camera)
		m_renderingManager->SetCamera(transform->GetPosition(), transform->GetRotation());

	if (light)
		m_renderingManager->SetLight(transform->GetPosition(), light->GetLightData());

	if (boxCollider && !rigidbody)
		m_toCollideWith.push_back(std::pair<Transform&, BoxCollider&>(*transform, *boxCollider));
	else if (boxCollider && rigidbody)
		m_toApplyPhysicOn.push_back(std::pair<Transform&, std::pair<BoxCollider&, Rigidbody&>>(*transform, std::pair<BoxCollider&, Rigidbody&>(*boxCollider, *rigidbody)));

	auto childs = p_entity.GetChilds();
	for (auto& child : childs)
		ParseSceneRecursive(*child);
}

void ElkGameEngine::Managers::EngineManager::HandleRendering()
{
	PROFILER_SPY("EngineManager::HandleRendering");

	m_windowManager->GetDriver()->ClearScreen();
	RenderScene();
	m_windowManager->GetDevice()->SwapBuffers();
}

void ElkGameEngine::Managers::EngineManager::HandleInputs()
{
	PROFILER_SPY("EngineManager::HandleInputs");

	m_inputManager->Update();
	Context::Device::PollEvents();
}

void ElkGameEngine::Managers::EngineManager::ShowFramerate()
{
	m_timer += ElkTools::Utils::Time::GetDeltaTime();
	if (m_timer >= 1.0f)
	{
		ElkTools::Debug::Log::Process("Framerate : " + std::to_string(ElkTools::Utils::Time::GetFramerate()) + " FPS");
		m_timer = 0.0f;
	}
}

std::string ElkGameEngine::Managers::EngineManager::BooleanToToggleState(bool p_state)
{
	if (p_state)
		return "ON";
	else
		return "OFF";
}

void ElkGameEngine::Managers::EngineManager::ToggleFramerateDisplay()
{
	m_showFramerate = !m_showFramerate;
	m_timer = 0.0f;
	ElkTools::Debug::Log::Process("Framerate counter : " + BooleanToToggleState(m_showFramerate), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkGameEngine::Managers::EngineManager::ToggleRendering()
{
	m_enableRendering = !m_enableRendering;
	ElkTools::Debug::Log::Process("Rendering : " + BooleanToToggleState(m_enableRendering), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkGameEngine::Managers::EngineManager::TogglePhysics()
{
	m_enablePhysics = !m_enablePhysics;
	ElkTools::Debug::Log::Process("Physics : " + BooleanToToggleState(m_enablePhysics), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkGameEngine::Managers::EngineManager::ToggleParsing()
{
	m_enableParsing = !m_enableParsing;
	ElkTools::Debug::Log::Process("Parsing : " + BooleanToToggleState(m_enableParsing), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkGameEngine::Managers::EngineManager::ToggleModelRendering()
{
	m_enableModelRendering = !m_enableModelRendering;
	ElkTools::Debug::Log::Process("Model rendering : " + BooleanToToggleState(m_enableModelRendering), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

void ElkGameEngine::Managers::EngineManager::ToggleColliderRendering()
{
	m_enableColliderRendering = !m_enableColliderRendering;
	ElkTools::Debug::Log::Process("Collider rendering : " + BooleanToToggleState(m_enableColliderRendering), ElkTools::Debug::Log::LogLevel::LOG_INFO);
}

std::shared_ptr<ElkGameEngine::Managers::SceneManager> ElkGameEngine::Managers::EngineManager::GetSceneManager() const
{
	return m_sceneManager;
}

std::shared_ptr<ElkGameEngine::Managers::InputManager> ElkGameEngine::Managers::EngineManager::GetInputManager() const
{
	return m_inputManager;
}

std::shared_ptr<ElkRendering::Managers::RenderingManager> ElkGameEngine::Managers::EngineManager::GetRenderingManager() const
{
	return m_renderingManager;
}

void ElkGameEngine::Managers::EngineManager::Setup()
{
	m_quit = false;

	m_windowManager = std::make_shared<ElkGameEngine::Managers::WindowManager>(m_sharedContext);
	m_sharedContext.windowManager = m_windowManager;

	m_sceneManager = std::make_shared<ElkGameEngine::Managers::SceneManager>(m_sharedContext);
	m_sharedContext.sceneManager = m_sceneManager;

	m_inputManager = std::make_shared<ElkGameEngine::Managers::InputManager>(m_sharedContext);
	m_sharedContext.eventManager = m_inputManager;

	m_renderingManager = std::make_shared<ElkRendering::Managers::RenderingManager>(m_windowManager->GetWidth(), m_windowManager->GetHeight());
	m_sharedContext.renderer = m_renderingManager;

	m_physicsManager = std::make_shared<ElkPhysics::Managers::PhysicsManager>();
	m_sharedContext.physicsManager = m_physicsManager;

	ElkTools::Debug::Log::Process("Engine setup completed", ElkTools::Debug::Log::LogLevel::LOG_INFO);
}
