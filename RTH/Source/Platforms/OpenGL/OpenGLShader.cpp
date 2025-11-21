#include "Rthpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace RTH
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "Vertex")
			return GL_VERTEX_SHADER;
		if (type == "Fragment" || type == "Pixel")
			return GL_FRAGMENT_SHADER;
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSrc = PreProcess(source);
		Compile(shaderSrc);

		// Assets/Shaders/Texture.glsl
		auto lastSlash = filePath.find_last_of("/\\"); 
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind(".");
		size_t count = (lastDot == std::string::npos) ? filePath.size() - lastSlash : lastDot - lastSlash;
		mName = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc) : mName(name)
	{
		std::unordered_map<GLenum, std::string> Src;
		Src[GL_VERTEX_SHADER] = vertexSrc;
		Src[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(Src);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream file(filePath, std::ios::in | std::ios::binary);
		if (file)
		{
			file.seekg(0, std::ios::end);
			result.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&result[0], result.size());
			file.close();
		}
		else
		{
			RTH_CORE_ERROR("Could open the file at '{0}'", filePath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSrc;
		const char* tokenType = "#type";
		size_t tokenTypeLength = strlen(tokenType);
		size_t pos = source.find(tokenType, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			RTH_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + tokenTypeLength + 1;
			std::string type = source.substr(begin, eol - begin);
			RTH_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(tokenType, nextLinePos);
			shaderSrc[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSrc;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSrc)
	{
		GLint program = glCreateProgram();
		RTH_CORE_ASSERT(shaderSrc.size() <= 2, "Shader source length is more than 2 dude!");
		std::array<GLenum, 2> glShaderIDs;
		int glArraydIndex = 0;
		for (auto& kv : shaderSrc)
		{
			GLenum type = kv.first;
			const std::string& src = kv.second;
			GLuint shader = glCreateShader(type);
			const GLchar* source = src.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				RTH_CORE_ERROR("Vertex shader failed to compile!");
				RTH_CORE_ERROR("{0}", infoLog.data());
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glArraydIndex++] = shader;
		}
		
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto id : glShaderIDs)
				glDeleteShader(id);

			RTH_CORE_ERROR("Shader failed to link!");
			RTH_CORE_ERROR("{0}", infoLog.data());
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		mRendererId = program;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererId);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(mRendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}