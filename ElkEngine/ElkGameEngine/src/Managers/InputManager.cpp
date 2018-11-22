#include "stdafxGameEngine.h"

#include "ElkGameEngine/Managers/InputManager.h"
#include "ElkGameEngine/Managers/WindowManager.h"

using namespace ElkGameEngine::Managers;

InputManager::KeyEventsMap InputManager::occuringKeyEvents;
InputManager::MouseEventsMap InputManager::occuringMouseEvents;

InputManager::InputManager(ElkGameEngine::Utils::SharedContext& p_sharedContext)
	: m_sharedContext(p_sharedContext)
{
	InputManager::Setup();
}

InputManager::~InputManager()
{
	InputManager::Close();
}

void InputManager::Setup() {}

void InputManager::Update()
{
	occuringKeyEvents.clear();
	occuringMouseEvents.clear();
}

void InputManager::Close() {}

bool InputManager::IsKeyEventOccured(const Key p_key, const KeyState p_keyEvent)
{
	if (occuringKeyEvents.find(p_key) == occuringKeyEvents.end())
		return false;

	return occuringKeyEvents[p_key] == p_keyEvent;
}

InputManager::KeyState InputManager::GetKeyState(Key p_key) const
{
	const auto result = m_sharedContext.windowManager->GetDevice()->GetKey(static_cast<int>(p_key));

	if (result == Context::Device::GetPressState())
		return KeyState::KEY_DOWN;

	return KeyState::KEY_UP;
}

bool InputManager::IsKeyDown(const Key p_key) const
{
	return GetKeyState(p_key) == KeyState::KEY_DOWN;
}

bool InputManager::IsKeyUp(const Key p_key) const
{
	return GetKeyState(p_key) == KeyState::KEY_UP;
}

bool InputManager::IsMouseEventOccured(MouseButton p_mouseButton, MouseButtonState p_mouseButtonState)
{
	if (occuringMouseEvents.find(p_mouseButton) == occuringMouseEvents.end())
		return false;

	return occuringMouseEvents[p_mouseButton] == p_mouseButtonState;
}

InputManager::MouseButtonState InputManager::GetMouseButtonState(MouseButton p_mouseButton) const
{
	const auto result = m_sharedContext.windowManager->GetDevice()->GetMouseButton(static_cast<int>(p_mouseButton));

	if (result == Context::Device::GetPressState())
		return MouseButtonState::MOUSE_DOWN;

	return MouseButtonState::MOUSE_UP;
}

bool InputManager::IsMouseButtonDown(const MouseButton p_mouseButton) const
{
	return GetMouseButtonState(p_mouseButton) == MouseButtonState::MOUSE_DOWN;
}

bool InputManager::IsMouseButtonUp(const MouseButton p_mouseButton) const
{
	return GetMouseButtonState(p_mouseButton) == MouseButtonState::MOUSE_UP;
}

glm::vec2 InputManager::GetCursorPosition() const
{
	const std::pair<double, double> mousePos = m_sharedContext.windowManager->GetDevice()->GetCursorPosition();
	return glm::vec2(static_cast<float>(mousePos.first), static_cast<float>(mousePos.second));
}


