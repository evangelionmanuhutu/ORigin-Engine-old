#include "pch.h"
#include "OpenGL_Shader.h"

#include "Origin\Utils\log.h"

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace Origin {

  OpenGLShader::OpenGLShader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0)
  {
    m_ShaderSource = ParseShader(filepath);
    m_RendererID = createShader(m_ShaderSource.VertexSources, m_ShaderSource.FragmentSources);

    // extract shader file to string name
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind(".");
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
  }

  OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
    : m_Name(name), m_Filepath(filepath), m_RendererID(0)
  {
    m_ShaderSource = ParseShader(filepath);
    m_RendererID = createShader(m_ShaderSource.VertexSources, m_ShaderSource.FragmentSources);
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_RendererID);
  }

  ShaderProgramSources OpenGLShader::ParseShader(const std::string& filePath)
  {
    std::ifstream stream(filePath);

    enum class shaderType
    {
      NONE = -1,
      VERTEX,
      FRAGMENT
    };

    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;

    while (getline(stream, line)) {

      if (line.find("//type ") != std::string::npos) {
        if (line.find("vertex") != std::string::npos)
          type = shaderType::VERTEX;
        else if (line.find("fragment") != std::string::npos)
          type = shaderType::FRAGMENT;
      }
      else ss[(int)type] << line << "\n";
    }
    return { ss[0].str(), ss[1].str() };
  }

  unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
  {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(id, 512, NULL, infoLog);
      std::cout << infoLog << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;

      glDeleteShader(id);
      return 0;
    }
    return id;
  }

  unsigned int OpenGLShader::createShader(const std::string& vertexShader, std::string& fragmentShader)
  {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
  }

  void OpenGLShader::Bind() const
  {
    glUseProgram(m_RendererID);
  }
  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

	void OpenGLShader::SetBool(const std::string& name, bool boolean)
	{
    SetUniformBool(name, boolean);
	}

	void OpenGLShader::SetFloat(const std::string& name, float v0)
	{
    SetUniformFloat(name, v0);
	}

	void OpenGLShader::SetFloat(const std::string& name, float v0, float v1)
	{
    SetUniformFloat(name, v0, v1);
	}

	void OpenGLShader::SetFloat(const std::string& name, float v0, float v1, float v2)
	{
    SetUniformFloat(name, v0, v1, v2);
	}

	void OpenGLShader::SetFloat(const std::string& name, float v0, float v1, float v2, float v3)
	{
    SetUniformFloat(name, v0, v1, v2, v3);
	}

	void OpenGLShader::SetInt(const std::string& name, int v0)
	{
    SetUniformInt(name, v0);
	}

	void OpenGLShader::SetInt(const std::string& name, int v0, int v1)
	{
    SetUniformInt(name, v0, v1);
	}

	void OpenGLShader::SetInt(const std::string& name, int v0, int v1, int v2)
	{
    SetUniformInt(name, v0, v1, v2);
	}

	void OpenGLShader::SetInt(const std::string& name, int v0, int v1, int v2, int v3)
	{
    SetUniformInt(name, v0, v1, v2, v3);
	}

  void  OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
  {
    SetUniformIntArray(name, values, count);
  }

	void OpenGLShader::SetVector(const std::string& name, const glm::vec2& vector2)
	{
    SetUniformVector(name, vector2);
	}

	void OpenGLShader::SetVector(const std::string& name, const glm::vec3& vector3)
	{
    SetUniformVector(name, vector3);
	}

	void OpenGLShader::SetVector(const std::string& name, const glm::vec4& vector4)
	{
    SetUniformVector(name, vector4);
	}

	void OpenGLShader::SetMatrix(const std::string& name, const glm::mat3& vector3)
	{
    SetUniformMatrix(name, vector3);
	}

	void OpenGLShader::SetMatrix(const std::string& name, const glm::mat4& vector4)
	{
    SetUniformMatrix(name, vector4);
	}


  // private 
	// BOOLEAN UNIFORM
  void OpenGLShader::SetUniformBool(const std::string& name, bool boolean) {
    glUniform1i(GetUniformLocation(name), boolean);
  }

  // FLOAT UNIFORM
  void OpenGLShader::SetUniformFloat(const std::string& name, float v0) {
    glUniform1f(GetUniformLocation(name), v0);
  }
  void OpenGLShader::SetUniformFloat(const std::string& name, float v0, float v1) {
    glUniform2f(GetUniformLocation(name), v0, v1);
  }
  void OpenGLShader::SetUniformFloat(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
  }
  void OpenGLShader::SetUniformFloat(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
  }
  // INT UNIFORM
  void OpenGLShader::SetUniformInt(const std::string& name, int v0) {
    glUniform1i(GetUniformLocation(name), v0);
  }

  void OpenGLShader::SetUniformInt(const std::string& name, int v0, int v1) {
    glUniform2i(GetUniformLocation(name), v0, v1);
  }

  void OpenGLShader::SetUniformInt(const std::string& name, int v0, int v1, int v2) {
    glUniform3i(GetUniformLocation(name), v0, v1, v2);
  }

  void OpenGLShader::SetUniformInt(const std::string& name, int v0, int v1, int v2, int v3) {
    glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
  }

  void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, uint32_t count)
  {
    glUniform1iv(GetUniformLocation(name), count, values);
  }

  // VECTOR UNIFORM
  void OpenGLShader::SetUniformVector(const std::string& name, const glm::vec2& vector2) {
    glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vector2));
  }

  void OpenGLShader::SetUniformVector(const std::string& name, const glm::vec3& vector3) {
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector3));
  }

  void OpenGLShader::SetUniformVector(const std::string& name, const glm::vec4& vector4) {
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vector4));
  }

  // MATRIX UNIFORM
  void OpenGLShader::SetUniformMatrix(const std::string& name, const glm::mat3& MATRIX) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(MATRIX));
  }

  void OpenGLShader::SetUniformMatrix(const std::string& name, const glm::mat4& MATRIX) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(MATRIX));
  }

  int OpenGLShader::GetUniformLocation(const std::string& name)
  {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
      return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    //std::cout << name << " location : " << location << "\n";
    if (location == -1)
      OGN_CORE_WARN("{0} : WARNING UNIFORM \"{1}\" does not exists or uninitialized", GetName(), name);

    m_UniformLocationCache[name] = location;
    return location;
  }
}