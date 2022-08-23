#pragma once
#include <memory>
#include "Origin\Core\OriginCore.h"

namespace Origin
{
	class Texture {
	public:
		virtual ~Texture() = default;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetSlot() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static std::shared_ptr<Texture2D> Create(const std::string& path);
	};
}