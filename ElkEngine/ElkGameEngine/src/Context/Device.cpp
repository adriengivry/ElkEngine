#include "stdafxGameEngine.h"

#include <ElkTools/Debug/Assertion.h>

#include "ElkGameEngine/Context/Device.h"

using namespace ElkGameEngine;

void Context::GLFWdeleter::operator()(GLFWwindow* ptr) const
{
	glfwDestroyWindow(ptr);
}

void Context::ErrorCallback(int, const char* p_err_str)
{
	ElkTools::Debug::Assert(true, "GLFW Error: " + std::string(p_err_str), __FILE_INFO__);
}

void Context::KeyCallback(GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods)
{
	if (p_action == GLFW_PRESS)
		Managers::InputManager::occuringKeyEvents[static_cast<Managers::InputManager::Key>(p_key)] = Managers::InputManager::KeyState::KEY_DOWN;

	if (p_action == GLFW_RELEASE)
		Managers::InputManager::occuringKeyEvents[static_cast<Managers::InputManager::Key>(p_key)] = Managers::InputManager::KeyState::KEY_UP;
}

void Context::MouseCallback(GLFWwindow* p_window, int p_mouseButton, int p_action, int p_mods)
{
	if (p_action == GLFW_PRESS)
		Managers::InputManager::occuringMouseEvents[static_cast<Managers::InputManager::MouseButton>(p_mouseButton)] = Managers::InputManager::MouseButtonState::MOUSE_DOWN;

	if (p_action == GLFW_RELEASE)
		Managers::InputManager::occuringMouseEvents[static_cast<Managers::InputManager::MouseButton>(p_mouseButton)] = Managers::InputManager::MouseButtonState::MOUSE_UP;
}

Context::Device::Device()
{
	m_width = 0;
	m_height = 0;
}

void Context::Device::Setup()
{
	glfwSetErrorCallback(ErrorCallback);

	const int result = glfwInit();

	ElkTools::Debug::Assert(result == GL_FALSE, "GLFW Error!", __FILE_INFO__);

	//- Setting to core profile mode ------
	// In Core profile mode, we need to explicit a VAO (no need in Compatibility mode)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_glfwWindow = std::unique_ptr<GLFWwindow, GLFWdeleter>(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr));
	
	ElkTools::Debug::Assert(!m_glfwWindow.get(), "GLFW can't create window", __FILE_INFO__);

	glfwMakeContextCurrent(m_glfwWindow.get());

	glfwSetKeyCallback(m_glfwWindow.get(), KeyCallback);
	glfwSetMouseButtonCallback(m_glfwWindow.get(), MouseCallback);

	// Force disable v-sync
	glfwSwapInterval(0);
}

void Context::Device::Close()
{
	glfwTerminate();
}

bool Context::Device::IsActive() { return !glfwWindowShouldClose(m_glfwWindow.get()); }

int Context::Device::GetKey(const int p_key) const
{
	return glfwGetKey(m_glfwWindow.get(), p_key);
}

int Context::Device::GetMouseButton(const int p_mouseButton) const
{
	return glfwGetMouseButton(m_glfwWindow.get(), p_mouseButton);
}

std::pair<double, double>& Context::Device::GetCursorPosition()
{
	glfwGetCursorPos(m_glfwWindow.get(), &m_cursorPosition.first, &m_cursorPosition.second);
	return m_cursorPosition;
}

int Context::Device::GetPressState()
{
	return GLFW_PRESS;
}

int Context::Device::GetReleaseState()
{
	return GLFW_RELEASE;
}

void Context::Device::PollEvents()
{
	glfwPollEvents();
}

void Context::Device::SwapBuffers()
{
	glfwSwapBuffers(m_glfwWindow.get());
}
