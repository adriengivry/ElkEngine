#include "stdafx.h"

#include <memory>

#include <ElkGameEngine/ElkGameEngine.h>

using namespace ElkGameEngine::Objects::Components;
using namespace ElkGameEngine::Objects::Components::Behaviours;

int main()
{
	ElkGameEngine::Managers::EngineManager elkGameEngine;

	elkGameEngine.GetRenderingManager()->SetRenderMode(ElkRendering::Managers::RenderingManager::RenderMode::NORMAL);

	std::shared_ptr<ElkGameEngine::Managers::SceneManager> sceneManager = elkGameEngine.GetSceneManager();

	auto& scene = sceneManager->CreateScene("MyScene");
	auto& player = sceneManager->CreateEmptyGameObject("Player");
	auto& floor = sceneManager->CreateEmptyGameObject("Floor");
	auto& cubeOnFloor = sceneManager->CreateEmptyGameObject("Cube On Floor");
	auto& camera = sceneManager->CreateEmptyGameObject("Camera");
	
	/* Init Player */
	auto playerModelRenderer = player.AddComponent<ModelRenderer>();
	playerModelRenderer->SetModel(elkGameEngine.GetRenderingManager()->GetModelManager().RequireAndGet("Nanosuit"));
	player.AddComponent<Physics::BoxCollider>()->SetExtend(glm::vec3(0.5f, 1.0f, 0.5f));
	auto playerRigidbody = player.AddComponent<Physics::Rigidbody>();
	auto playerTransform = player.GetComponent<Transform>();
	playerTransform->Translate(glm::vec3(0.0f, 25.0f, 0.0f));
	playerTransform->Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	/* Init floor */
	auto floorModelRenderer = floor.AddComponent<ModelRenderer>();
	floorModelRenderer->SetModel(elkGameEngine.GetRenderingManager()->GetModelManager().RequireAndGet("Cube"));
	floor.AddComponent<Physics::BoxCollider>()->SetExtend(glm::vec3(2.5f, 0.5f, 2.5f));
	floor.GetComponent<Transform>()->Scale(glm::vec3(5.0f, 1.0f, 5.0f));
	floor.GetComponent<Transform>()->Translate(glm::vec3(0.0f, -3.0f, 0.0f));

	/* Init cube on floor */
	auto cubeOnFloorRenderer = cubeOnFloor.AddComponent<ModelRenderer>();
	cubeOnFloorRenderer->SetModel(elkGameEngine.GetRenderingManager()->GetModelManager().RequireAndGet("Cube"));
	cubeOnFloor.AddComponent<Physics::BoxCollider>()->SetExtend(glm::vec3(0.5f, 1.0f, 0.5f));
	cubeOnFloor.GetComponent<Transform>()->Scale(glm::vec3(1.0f, 2.0f, 1.0f));
	cubeOnFloor.GetComponent<Transform>()->Translate(glm::vec3(-2.0f, -1.5f, 0.0f));

	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());
	std::uniform_int_distribution<> dis(-5, 5);

	for (uint64_t i = 0; i < 25; ++i)
	{
		auto& cube = sceneManager->CreateEmptyGameObject("Cube");
		auto cubeModelRenderer = cube.AddComponent<ModelRenderer>();
		cubeModelRenderer->SetModel(elkGameEngine.GetRenderingManager()->GetModelManager().RequireAndGet("Cube"));
		cube.AddComponent<Physics::BoxCollider>()->SetExtend(glm::vec3(0.5f, 0.5f, 0.5f));
		cube.GetComponent<Transform>()->Translate(glm::vec3(dis(gen), dis(gen), dis(gen) - 20.0f));
	}

	/* Init Camera */
	camera.AddComponent<Camera>();
	auto cameraTransform = camera.GetComponent<Transform>();
	cameraTransform->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	cameraTransform->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
	auto cameraLightComponent = camera.AddComponent<Light>();

	float cameraNormalSpeed			= 5.0f;
	float cameraRunSpeed			= 10.0f;
	float cameraCurrentSpeed		= 0.0f;
	float playerRotationSpeed		= 360.0f;
	float cameraRotationSpeed		= 180.0f;
	float playerMovementSpeed		= 1.0f;

	while (elkGameEngine.IsRunning())
	{
		elkGameEngine.PreUpdate();

		/* Handle engine actions */
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_ESCAPE))
			elkGameEngine.Quit();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_F1, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.ToggleFramerateDisplay();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_1, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.ToggleRendering();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_2, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.TogglePhysics();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_3, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.ToggleParsing();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_4, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.ToggleColliderRendering();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_5, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.ToggleModelRendering();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_F5, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.GetRenderingManager()->GetShaderManager().RecompileShaders();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_B, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.GetBenchmark().Start();
		if (elkGameEngine.GetInputManager()->IsKeyEventOccured(ElkGameEngine::Managers::InputManager::Key::KEY_P, ElkGameEngine::Managers::InputManager::KeyState::KEY_DOWN))
			elkGameEngine.GetProfiler().Toggle();

		/* Handle player rotation */
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_Z))
			playerTransform->Rotate(glm::vec3(0, -playerRotationSpeed, 0) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_X))
			playerTransform->Rotate(glm::vec3(0, playerRotationSpeed, 0) * ElkTools::Utils::Time::GetDeltaTime());

		/* Handle player movement */
		glm::vec3 playerNewVelocity = glm::vec3(0.0f, playerRigidbody->GetVelocity().y, 0.0f);
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_LEFT))
			playerNewVelocity += playerMovementSpeed * glm::vec3(-1, 0, 0);
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_RIGHT))
			playerNewVelocity += playerMovementSpeed * glm::vec3(1, 0, 0);
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_UP))
			playerNewVelocity += playerMovementSpeed * glm::vec3(0, 0, -1);
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_DOWN))
			playerNewVelocity += playerMovementSpeed * glm::vec3(0, 0, 1);
		playerRigidbody->SetVelocity(playerNewVelocity);

		/* Handle camera movement */
		cameraCurrentSpeed = elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_LEFT_SHIFT) ? cameraRunSpeed : cameraNormalSpeed;
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_W))
			cameraTransform->Translate(glm::vec3(0, 0, -cameraCurrentSpeed) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_S))
			cameraTransform->Translate(glm::vec3(0, 0, cameraCurrentSpeed) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_A))
			cameraTransform->Translate(glm::vec3(-cameraCurrentSpeed, 0, 0) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_D))
			cameraTransform->Translate(glm::vec3(cameraCurrentSpeed, 0, 0) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_Q))
			cameraTransform->Translate(glm::vec3(0, -cameraCurrentSpeed, 0) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_E)
			|| elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_SPACE))
			cameraTransform->Translate(glm::vec3(0, cameraCurrentSpeed, 0) * ElkTools::Utils::Time::GetDeltaTime());

		/* Handle camera rotation */
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_J))
			cameraTransform->Rotate(glm::vec3(0.0f, -cameraRotationSpeed, 0.0f) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_L))
			cameraTransform->Rotate(glm::vec3(0.0f, cameraRotationSpeed, 0.0f) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_I))
			cameraTransform->Rotate(glm::vec3(cameraRotationSpeed, 0.0f, 0.0f) * ElkTools::Utils::Time::GetDeltaTime());
		if (elkGameEngine.GetInputManager()->IsKeyDown(ElkGameEngine::Managers::InputManager::Key::KEY_K))
			cameraTransform->Rotate(glm::vec3(-cameraRotationSpeed, 0.0f, 0.0f) * ElkTools::Utils::Time::GetDeltaTime());

		elkGameEngine.PostUpdate();
	}

	return EXIT_SUCCESS;
}
