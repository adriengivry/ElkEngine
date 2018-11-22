#pragma once

#include <memory>

#include "ElkGameEngine/export.h"

#include "ElkGameEngine/Managers/WindowManager.h"
#include "ElkGameEngine/Managers/SceneManager.h"
#include "ElkGameEngine/Managers/InputManager.h"

#include "ElkGameEngine/Utils/SharedContext.h"

#include "ElkGameEngine/Objects/GameObject.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ACustomComponent.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/ACollider.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/Rigidbody.h"
#include "ElkGameEngine/Objects/Components/Behaviours/ModelRenderer.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Camera.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Light.h"
#include "ElkGameEngine/Objects/Components/Transform.h"
#include "ElkGameEngine/Objects/Components/Behaviours/Physics/BoxCollider.h"

#include <ElkTools/Debug/Log.h>
#include <ElkTools/Debug/Assertion.h>
#include <ElkTools/Utils/Time.h>
#include <ElkTools/Utils/Benchmark.h>
#include <ElkTools/Utils/Profiler.h>
#include <ElkTools/Utils/Date.h>

namespace ElkGameEngine
{
	namespace Managers
	{
		/*
		* The Engine is the main class of the program, it calls everyone methods (Create Window, call Renderer update, etc...)
		*/
		class ELK_GAMEENGINE_EXPORT EngineManager : public ElkAPI::IManager
		{
		public:
			EngineManager() : m_benchmark(m_enableRendering, m_enablePhysics) { EngineManager::Setup(); }
			~EngineManager() { EngineManager::Close(); }

			void Quit();
			bool IsRunning() const;

			void PreUpdate();
			void PostUpdate();

			void ToggleFramerateDisplay();
			void ToggleRendering();
			void TogglePhysics();
			void ToggleParsing();
			void ToggleColliderRendering();
			void ToggleModelRendering();

			ElkTools::Utils::Benchmark& GetBenchmark() { return m_benchmark; }
			ElkTools::Utils::Profiler& GetProfiler() { return m_profiler; }
			std::shared_ptr<ElkGameEngine::Managers::SceneManager>		GetSceneManager() const;
			std::shared_ptr<ElkGameEngine::Managers::InputManager>		GetInputManager() const;
			std::shared_ptr<ElkRendering::Managers::RenderingManager>	GetRenderingManager() const;

		private:
			void Setup() override;
			void Close() override;

			void UpdatePhysics();
			void RenderScene();
			void ParseScene();
			void ParseSceneRecursive(Objects::GameObject& p_entity);
			void HandleRendering();
			void HandleInputs();
			void ShowFramerate();

			std::string BooleanToToggleState(bool p_state);

			Utils::SharedContext m_sharedContext;

			ElkTools::Utils::Benchmark m_benchmark;
			ElkTools::Utils::Profiler m_profiler;
			ElkTools::Utils::Time m_time;
			
			std::shared_ptr<ElkGameEngine::Managers::InputManager>		m_inputManager;
			std::shared_ptr<ElkPhysics::Managers::PhysicsManager>		m_physicsManager;
			std::shared_ptr<ElkGameEngine::Managers::SceneManager>		m_sceneManager;
			std::shared_ptr<ElkRendering::Managers::RenderingManager>	m_renderingManager;
			std::shared_ptr<ElkGameEngine::Managers::WindowManager>		m_windowManager;

			std::vector<std::pair<Objects::Components::Transform&, Objects::Components::Behaviours::ModelRenderer&>> m_toDraw;
			std::vector<std::pair<Objects::Components::Transform&, Objects::Components::Behaviours::Physics::BoxCollider&>> m_toDrawCollider;
			std::vector<std::pair<Objects::Components::Transform&, Objects::Components::Behaviours::Physics::BoxCollider&>> m_toCollideWith;
			std::vector<std::pair<Objects::Components::Transform&, std::pair<Objects::Components::Behaviours::Physics::BoxCollider&, Objects::Components::Behaviours::Physics::Rigidbody&>>> m_toApplyPhysicOn;

			bool	m_quit;
			
			bool	m_showFramerate = false;
			double	m_timer			= 0.0f;

			bool	m_enableRendering			= true;
			bool	m_enablePhysics				= true;
			bool	m_enableParsing				= true;
			bool	m_enableModelRendering		= true;
			bool	m_enableColliderRendering	= false;
		};
	}
}
