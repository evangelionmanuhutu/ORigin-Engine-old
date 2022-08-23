#pragma once
#include "Origin\Renderer\Texture.h"
#include <glad\glad.h>

namespace Origin {

  class OpenGLTexture2D : public Texture2D
  {
  private:
    uint32_t m_RendererID;
    std::string m_FilePath;
    uint32_t m_Width, m_Height, m_BPP;
    uint32_t m_Slot;
    GLenum m_InternalFormat, m_DataFormat;

  public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    virtual void SetData(void* data, uint32_t size) override;

    virtual void Bind(uint32_t slot = 0) override;
    virtual void Unbind() const override;

    virtual uint32_t GetRendererID() const override { return m_RendererID; }

    virtual uint32_t GetSlot() const override { return m_Slot; }

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override{ return m_Height; }

    virtual bool operator== (const Texture& other) const override
    {
      return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
    }
  };

}
