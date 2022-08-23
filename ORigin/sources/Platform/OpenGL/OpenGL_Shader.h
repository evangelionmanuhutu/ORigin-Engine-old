#pragma once
#include "pch.h"
#include "Origin\Renderer\Shader.h"

namespace Origin
{
  struct ShaderProgramSources {

    std::string VertexSources;
    std::string FragmentSources;
  };

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& filepath);

    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() const override {  return m_Name; }

    virtual std::string GetFile() const override {
      std::string source = (m_Filepath + "\n" +
        "\nVertex\n" + m_ShaderSource.VertexSources +
        "Fragment\n" + m_ShaderSource.FragmentSources); 
      return source; }

  private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    ShaderProgramSources m_ShaderSource;
    std::string m_Name;

  public:

		// BOOLEAN UNIFORM
		virtual void SetBool(const std::string& name, bool boolean) override;

		// FLOAT UNIFORM
		virtual void SetFloat(const std::string& name, float v0) override;
		virtual void SetFloat(const std::string& name, float v0, float v1) override;
		virtual void SetFloat(const std::string& name, float v0, float v1, float v2) override;
		virtual void SetFloat(const std::string& name, float v0, float v1, float v2, float v3) override;

		// INT UNIFORM
		virtual void SetInt(const std::string& name, int v0) override;
		virtual void SetInt(const std::string& name, int v0, int v1) override;
		virtual void SetInt(const std::string& name, int v0, int v1, int v2) override;
		virtual void SetInt(const std::string& name, int v0, int v1, int v2, int v3) override;

    virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		// VECTOR UNIFORM
		virtual void SetVector(const std::string& name, const glm::vec2& vector2)override;
		virtual void SetVector(const std::string& name, const glm::vec3& vector3) override;
		virtual void SetVector(const std::string& name, const glm::vec4& vector4) override;

		// MATRIX UNIFORM
		virtual void SetMatrix(const std::string& name, const glm::mat3& vector3) override;
		virtual void SetMatrix(const std::string& name, const glm::mat4& vector4) override;

  private:
    // BOOLEAN UNIFORM
    void SetUniformBool(const std::string& name, bool boolean);

    // FLOAT UNIFORM
    void SetUniformFloat(const std::string& name, float v0);
    void SetUniformFloat(const std::string& name, float v0, float v1);
    void SetUniformFloat(const std::string& name, float v0, float v1, float v2);
    void SetUniformFloat(const std::string& name, float v0, float v1, float v2, float v3);

    // INT UNIFORM
    void SetUniformInt(const std::string& name, int v0);
    void SetUniformInt(const std::string& name, int v0, int v1);
    void SetUniformInt(const std::string& name, int v0, int v1, int v2);
    void SetUniformInt(const std::string& name, int v0, int v1, int v2, int v3);

    void SetUniformIntArray(const std::string& name, int* values, uint32_t count);

    // VECTOR UNIFORM
		void SetUniformVector(const std::string& name, const glm::vec2& vector2);
    void SetUniformVector(const std::string& name, const glm::vec3& vector3);
    void SetUniformVector(const std::string& name, const glm::vec4& vector4);

    // MATRIX UNIFORM
    void SetUniformMatrix(const std::string& name, const glm::mat3& MATRIX);
    void SetUniformMatrix(const std::string& name, const glm::mat4& MATRIX);

  private:
    ShaderProgramSources ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
  };
}