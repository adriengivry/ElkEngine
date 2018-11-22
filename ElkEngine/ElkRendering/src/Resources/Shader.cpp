#include "stdafxRendering.h"

#include <ElkTools/Debug/Log.h>
#include <ElkTools/Utils/Profiler.h>

#include "ElkRendering/Resources/Shader.h"
#include "ElkRendering/Debug/GLDebug.h"

using namespace ElkRendering::Resources;

std::string Shader::m_vertexShaderStart("#shader vertex");
std::string Shader::m_fragmentShaderStart("#shader fragment");

Shader::Shader()
	: m_rendererID(0)
{
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	: m_rendererID(0)
{
	const std::string vertexShaderSource = ParseTwoFiles(vertexShader);
	const std::string fragmentShaderSource = ParseTwoFiles(fragmentShader);

	GLCall(const unsigned int program = glCreateProgram());
	const unsigned int vs = Compile(GL_VERTEX_SHADER, vertexShaderSource);
	const unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	m_rendererID = program;
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererID));
}

bool Shader::LoadFromFile(const std::string& p_path)
{
	PROFILER_SPY("Shader::LoadFromFile");

	m_sourceFile = p_path;
	const ShaderProgramSource sources = ParseOneFile(std::move(p_path));
	bool result = Compile(sources);
	auto logLevel = result ? ElkTools::Debug::Log::LogLevel::LOG_INFO : ElkTools::Debug::Log::LogLevel::LOG_ERROR;
	ElkTools::Debug::Log::Process("Compiling " + m_sourceFile + ": " + (result ? "SUCCESS" : "FAILED"), logLevel);
	return result;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

bool ElkRendering::Resources::Shader::Recompile()
{
	return LoadFromFile(m_sourceFile);
}

ShaderProgramSource Shader::ParseOneFile(const std::string&& p_filepath)
{
	std::ifstream shaderFile;
	shaderFile.open(p_filepath);
	std::string shader;
	ShaderProgramSource sources;

	if (shaderFile.is_open())
	{
		shaderFile.seekg(0, std::ios::end);
		shader.reserve(shaderFile.tellg());
		shaderFile.seekg(0, std::ios::beg);

		shader.assign(std::istreambuf_iterator<char>(shaderFile),
		              std::istreambuf_iterator<char>());
	}
	else
	{
		ElkTools::Debug::Log::Process("Error reading shader file : " + p_filepath, ElkTools::Debug::Log::LogLevel::LOG_ERROR);
		shaderFile.close();
		return {};
	}
	shaderFile.close();

	const GLsizei vtStartIndex = (GLsizei)(shader.find(m_vertexShaderStart) + m_vertexShaderStart.size());
	const GLsizei fragStartIndex = (GLsizei)shader.find(m_fragmentShaderStart);

	if (shader.size() != 0 && vtStartIndex != 0 && fragStartIndex != 0)
	{
		sources.vertexSource = shader.substr(vtStartIndex, fragStartIndex - vtStartIndex);
		sources.fragmentSource = shader.substr(fragStartIndex + m_fragmentShaderStart.size());
	}
	else
	{
		ElkTools::Debug::Log::Process("Error finding shaders in file : " + p_filepath, ElkTools::Debug::Log::LogLevel::LOG_ERROR);
	}

	return sources;
}

std::string Shader::ParseTwoFiles(const std::string& filePath) const
{
	std::ifstream stream(filePath);
	std::string line;
	std::stringstream stringstream;

	while (getline(stream, line))
	{
		stringstream << line << "\n";
	}

	return stringstream.str();
}

bool Shader::Compile(const ShaderProgramSource& p_sources)
{
	if (!p_sources.vertexSource.empty() && !p_sources.fragmentSource.empty())
	{
		const char* vtShaderChar = p_sources.vertexSource.c_str();
		const char* fragShaderChar = p_sources.fragmentSource.c_str();

		const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vertexShader, 1, &vtShaderChar, nullptr));
		GLCall(glCompileShader(vertexShader));

		if (CatchCompilerError(GL_COMPILE_STATUS, vertexShader))
		{
			GLCall(glDeleteShader(vertexShader));
			return false;
		}

		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fragmentShader, 1, &fragShaderChar, nullptr));
		GLCall(glCompileShader(fragmentShader));

		if (CatchCompilerError(GL_COMPILE_STATUS, fragmentShader))
		{
			GLCall(glDeleteShader(vertexShader));
			GLCall(glDeleteShader(fragmentShader));
			return false;
		}

		m_rendererID = glCreateProgram();
		GLCall(glAttachShader(m_rendererID, vertexShader));
		GLCall(glAttachShader(m_rendererID, fragmentShader));
		GLCall(glLinkProgram(m_rendererID));

		//if (CatchCompilerError(GL_LINK_STATUS, m_rendererID))
		//{
		//	GLCall(glDeleteShader(vertexShader));
		//	GLCall(glDeleteShader(fragmentShader));
		//	return false;
		//}

		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));

		return true;
	}
	return false;
}

int Shader::Compile(const unsigned int type, const std::string& source) const
{
	const char* src = source.c_str();
	int result;

	GLCall(const unsigned int id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		auto* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex" : "fragment") <<
			" shader \n";
		std::cout << message << "\n";

		GLCall(glDeleteShader(id));

		return 0;
	}

	return id;
}

bool Shader::CatchCompilerError(const GLenum p_compilerAction, const GLuint p_compiledShader) const
{
	int success;
	int length = 0;
	char* err = nullptr;

	GLCall(glGetShaderiv(p_compiledShader, p_compilerAction, &success));
	if (success == GL_FALSE)
	{
		GLCall(glGetShaderiv(p_compiledShader, GL_INFO_LOG_LENGTH, &length));
		err = static_cast<char*>(alloca(length));
		GLCall(glGetShaderInfoLog(p_compiledShader, length, nullptr, err));
		ElkTools::Debug::Log::Process("Shader " + std::string((p_compilerAction == GL_COMPILE_STATUS) ? "Compilation" : "Linking")
			+ " failed : " + err, ElkTools::Debug::Log::LogLevel::LOG_ERROR);

		return true;
	}

	return false;
}

int Shader::GetUniformLocation(const std::string& p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	GLCall(const int location = glGetUniformLocation(m_rendererID, p_name.c_str()));
	if (location == -1)
		ElkTools::Debug::Log::Process("Uniform " + p_name + " doesn't exist", ElkTools::Debug::Log::LogLevel::LOG_WARNING);
	//ASSERT(location != -1);

	m_uniformLocationCache[p_name] = location;

	return location;
}


/* Uniforms */
void Shader::SetUniform(const std::string&& p_name, const int p_value)
{
	GLCall(glUniform1i(GetUniformLocation(std::move(p_name)), p_value));
}

void Shader::SetUniform(const std::string&& p_name, const float p_value)
{
	GLCall(glUniform1f(GetUniformLocation(std::move(p_name)), p_value));
}

void Shader::SetUniform(const std::string&& p_name, const glm::vec2& p_vec2)
{
	GLCall(glUniform2f(GetUniformLocation(std::move(p_name)), p_vec2.x, p_vec2.y));
}

void Shader::SetUniform(const std::string&& p_name, const glm::vec3& p_vec3)
{
	GLCall(glUniform3f(GetUniformLocation(std::move(p_name)), p_vec3.x, p_vec3.y, p_vec3.z));
}

void Shader::SetUniform(const std::string&& p_name, const glm::vec4& p_vec4)
{
	GLCall(glUniform4f(GetUniformLocation(std::move(p_name)), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w));
}

void Shader::SetUniform(const std::string&& p_name, const glm::mat4& p_mat4)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(std::move(p_name)), 1, GL_FALSE, &p_mat4[0][0]));
}