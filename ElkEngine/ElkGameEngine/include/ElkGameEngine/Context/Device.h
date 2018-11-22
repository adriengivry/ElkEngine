#pragma once

#include <ElkAPI/IContext.h>

#include "ElkGameEngine/export.h"

#include "ElkGameEngine/Managers/InputManager.h"

namespace ElkGameEngine
{
	namespace Context
	{
		/*
		* Struct definition of the instance that will be given to the std::unique_pointer to handle destruction of GLFWwindow
		*/
		struct GLFWdeleter
		{
			/*
			* Method for the GLFWwindow to handle destruction
			*/
			void operator()(GLFWwindow* ptr) const;
		};

		/*
		* Callback method to handle error display with the GLFW context
		*/
		void ErrorCallback(int, const char* p_err_str);

		/*
		* Callback method to handle key events
		*/
		void KeyCallback(GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods);

		/*
		* Callback method to handle mouse events
		*/
		void MouseCallback(GLFWwindow* p_window, int p_mouseButton, int p_action, int p_mods);

		/*
		* The Device context handle the Window. Actually it uses GLFW Libarary
		*/
		class  Device : public ElkAPI::IContext
		{
		public:
			Device();
			virtual ~Device() = default;

			void Setup() override;
			void Close() override;

			void SetWidth(const int p_width) { m_width = p_width; }
			void SetHeight(const int p_height) { m_height = p_height; }
			void SetTitle(const std::string& p_title) { m_title = p_title; }

			bool IsActive() override;

			int GetKey(int p_key) const;
			int GetMouseButton(int p_mouseButton) const;
			std::pair<double, double>& GetCursorPosition();

			static int GetPressState();
			static int GetReleaseState();

			static void PollEvents();

			void SwapBuffers();

			GLFWwindow* GetContextWindow() const { return m_glfwWindow.get(); }

		private:
			std::unique_ptr<GLFWwindow, GLFWdeleter> m_glfwWindow;

			int							m_width;
			int							m_height;
			std::string					m_title;
			std::pair<double, double>	m_cursorPosition;
		};
	}
}
