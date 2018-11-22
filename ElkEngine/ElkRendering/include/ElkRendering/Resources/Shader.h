#pragma once

namespace ElkRendering
{
	namespace Resources
	{
		struct ShaderProgramSource
		{
			std::string vertexSource;
			std::string fragmentSource;
		};

		class Shader
		{
		private:
			static std::string m_vertexShaderStart;
			static std::string m_fragmentShaderStart;

			std::unordered_map<std::string, int> m_uniformLocationCache;
			unsigned int m_rendererID;

		public:
			Shader();
			Shader(const std::string& p_vertexShader, const std::string& p_fragmentShader);
			~Shader();

			bool LoadFromFile(const std::string& p_path);

			void Bind() const;
			void Unbind() const;

			inline unsigned int GetRendererID() const { return m_rendererID; }

			/* Uniforms */
			void SetUniform(const std::string&& p_name, const int p_value);
			void SetUniform(const std::string&& p_name, const float p_value);
			void SetUniform(const std::string&& p_name, const glm::vec2& p_vec2);
			void SetUniform(const std::string&& p_name, const glm::vec3& p_vec3);
			void SetUniform(const std::string&& p_name, const glm::vec4& p_vec4);
			void SetUniform(const std::string&& p_name, const glm::mat4& p_mat4);

			static inline void SetVertexShaderDelimiter(const std::string&& p_shaderStart)
			{
				m_vertexShaderStart = p_shaderStart;
			}

			static inline void SetFragmentShaderDelimiter(const std::string&& p_shaderStart)
			{
				m_fragmentShaderStart = p_shaderStart;
			}

			bool Recompile();
			std::string& GetSourceFile() { return m_sourceFile; }

		private:
			ShaderProgramSource ParseOneFile(const std::string&& p_filepath);
			std::string ParseTwoFiles(const std::string& filePath) const;

			bool Compile(const ShaderProgramSource& p_sources);
			int Compile(unsigned, const std::string& source) const;
			bool CatchCompilerError(const GLenum p_compilerAction, const unsigned p_compiledShader) const;
			int GetUniformLocation(const std::string& name);

			std::string m_sourceFile;
		};
	}
}
