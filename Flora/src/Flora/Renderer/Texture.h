#pragma once

namespace Flora {
	enum class ImageFormat {
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGB32F,
	};

	enum class ImageFilter {
		NEAREST = 0,
		LINEAR = 1,
	};

	struct TextureSpecification {
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		ImageFilter Filter = ImageFilter::NEAREST;
	};

	class Texture {
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const TextureSpecification& spec);
		static Ref<Texture2D> Create(const std::string& path);
	};
}