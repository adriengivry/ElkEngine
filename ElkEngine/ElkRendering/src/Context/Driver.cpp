#include "stdafxRendering.h"

#include <ElkTools/Debug/Log.h>
#include <ElkTools/Utils/IniLoader.h>

#include "ElkRendering/Context/Driver.h"
#include "ElkRendering/Debug/GLDebug.h"

using namespace ElkRendering::Context;
using namespace ElkRendering::Debug;

Driver::Driver()
{
	m_active = false;
}

void Driver::Setup()
{
	const GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		ElkTools::Debug::Log::Process("Error init glew : " +
			std::string(reinterpret_cast<const char*>(glewGetErrorString(error))), ElkTools::Debug::Log::LogLevel::LOG_ERROR);
	}

	GLCall(glEnable(GL_DEBUG_OUTPUT));
	GLCall(glDebugMessageCallback(GLDebug::DebugCallbackError, nullptr));
	GLCall(glEnable(GL_DEPTH_TEST));

	float clearRed		= ElkTools::Utils::IniLoader::GetFloat("rendering.ini", "clear_red");
	float clearGreen	= ElkTools::Utils::IniLoader::GetFloat("rendering.ini", "clear_green");
	float clearBlue		= ElkTools::Utils::IniLoader::GetFloat("rendering.ini", "clear_blue");
	float clearAlpha	= ElkTools::Utils::IniLoader::GetFloat("rendering.ini", "clear_alpha");

	GLCall(glClearColor(static_cast<GLclampf>(clearRed), static_cast<GLclampf>(clearGreen), static_cast<GLclampf>(clearBlue), static_cast<GLclampf>(clearAlpha)));

	DisplayDriverInfo();

	m_active = true;
}

bool Driver::IsActive() { return m_active; }

void ElkRendering::Context::Driver::DisplayDriverInfo()
{
	std::vector<std::string> openglInfo
	{
		std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))),
		"OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))),
		"GLSL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)))
	};

	for (auto& info : openglInfo)
		ElkTools::Debug::Log::Process(info, ElkTools::Debug::Log::LogLevel::LOG_DEFAULT, ElkTools::Debug::Log::LogColor::GREEN);
	ElkTools::Debug::Log::LineBreak(2);
}

void Driver::ClearScreen()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}