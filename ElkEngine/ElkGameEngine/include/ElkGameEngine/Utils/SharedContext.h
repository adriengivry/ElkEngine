#pragma once

#include <ElkRendering/Managers/RenderingManager.h>
#include <ElkPhysics/Managers/PhysicsManager.h>

namespace ElkGameEngine
{
	namespace Managers
	{
		class WindowManager;
		class SceneManager;
		class InputManager;
	}
}


namespace ElkGameEngine
{
	namespace Utils
	{
		/*
		* The SharedContext is a set of data that can be shared between class in order to allow some classes to get informations from other classes.
		* It also includes other struct (ISharedData) that are kind of boxes to organize informations
		*/
		struct SharedContext
		{
			std::shared_ptr<ElkGameEngine::Managers::WindowManager>			windowManager;
			std::shared_ptr<ElkGameEngine::Managers::SceneManager>			sceneManager;
			std::shared_ptr<ElkGameEngine::Managers::InputManager>			eventManager;
			std::shared_ptr<ElkRendering::Managers::RenderingManager>		renderer;
			std::shared_ptr<ElkPhysics::Managers::PhysicsManager>			physicsManager;
		};
	}
}
