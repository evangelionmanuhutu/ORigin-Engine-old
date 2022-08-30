#include "pch.h"
#include "stb_image.h"
#include "OpenGL_Texture.h"
#include "Origin\Core\OriginCore.h"

namespace Origin
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
	{
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

  OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_Slot(0)
  {
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = nullptr;
    data = stbi_load(path.c_str(),  &width, &height, &bpp, 0);

    OGN_CORE_ASSERT(data, "Failed to load texture!");

    m_Width = width;
    m_Height = height;
    m_BPP = bpp;

    GLenum internalFormat = 0, dataFormat = 0;
    if (m_BPP == 4)
    {
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGBA;
    }
    else if (m_BPP == 3)
    {
      internalFormat = GL_RGB8;
      dataFormat = GL_RGB;
    }

    m_InternalFormat = internalFormat;
    m_DataFormat = dataFormat;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

    if (data)
      stbi_image_free(data);
  }

	OpenGLTexture2D::~OpenGLTexture2D() {
    Unbind();
    glDeleteTextures(1, &m_RendererID);
  }

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    OGN_CORE_ASSERT(size == m_Width * m_Height * bpp, "data must be entire texture!");
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) {
    for (uint32_t index = 0; index <= slot; index++) {
      m_Slot = index;
      glBindTextureUnit(index, m_RendererID);
    }
  }
  void OpenGLTexture2D::Unbind() const {
    glBindTextureUnit(GL_TEXTURE_2D, 0);
  }
}