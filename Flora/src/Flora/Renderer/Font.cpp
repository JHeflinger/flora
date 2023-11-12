#include "flpch.h"
#include "Font.h"
#include "Flora/Renderer/Texture.h"
#include "Flora/Core/HiddenStructs.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"
#include "msdfgen.h"

namespace Flora {

    template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
    static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize,
        const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& geometry,
        uint32_t width, uint32_t height) {
        msdf_atlas::GeneratorAttributes attributes;
        attributes.config.overlapSupport = true;
        attributes.scanlinePass = true;
        msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
        generator.setAttributes(attributes);
        generator.setThreadCount(8);
        generator.generate(glyphs.data(), (int)glyphs.size());
        msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();
        TextureSpecification spec;
        spec.Width = bitmap.width;
        spec.Height = bitmap.height;
        spec.Format = ImageFormat::RGB8;
        spec.Filter = ImageFilter::LINEAR;
        Ref<Texture2D> texture = Texture2D::Create(spec);
        texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 4);
        return texture;
    }

	Font::Font(const std::filesystem::path& filepath)
        : m_Data(new FontData()) {
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
        if (ft) {
            msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.string().c_str());
            if (font) {

                struct CharsetRange { uint32_t Begin, End; };

                // from imgui_draw.cpp
                static const CharsetRange charsetRanges[] = { { 0x0020, 0x00FF } };

                msdf_atlas::Charset charset;
                for (CharsetRange range : charsetRanges)
                    for (uint32_t c = range.Begin; c <= range.End; c++)
                        charset.add(c);

                double fontscale = 1.0f;
                m_Data->Geometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
                int loaded = m_Data->Geometry.loadCharset(font, fontscale, charset);
                FL_CORE_INFO("Loaded {} glyphs", loaded);

                double emSize = 40.0;
                msdf_atlas::TightAtlasPacker atlasPacker;
                //atlasPacker.setDimensionsConstraint();
                atlasPacker.setPixelRange(2.0);
                atlasPacker.setMiterLimit(1.0);
                atlasPacker.setPadding(0);
                atlasPacker.setScale(emSize);
                int remaining = atlasPacker.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
                FL_CORE_INFO("remaining: {}", remaining);
                int width, height;
                atlasPacker.getDimensions(width, height);
                emSize = atlasPacker.getScale();

                #define DEFAULT_ANGLE_THRESHOLD 3.0
                #define LCG_MULTIPLIER 6364136223846793005ull
                #define LCG_INCREMENT 1442695040888963407ull
                #define THREAD_COUNT 8

                uint64_t coloringSeed = 0;
                bool expensiveColoring = false;
                if (expensiveColoring) {
                    msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
                        unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
                        glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                        return true;
                    }, m_Data->Glyphs.size()).finish(THREAD_COUNT);
                } else {
                    unsigned long long glyphSeed = coloringSeed;
                    for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs) {
                        glyphSeed *= LCG_MULTIPLIER;
                        glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                    }
                }

                #undef DEFAULT_ANGLE_THRESHOLD 
                #undef LCG_MULTIPLIER
                #undef LCG_INCREMENT
                #undef THREAD_COUNT

                m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)emSize, m_Data->Glyphs, m_Data->Geometry, width, height);

                msdfgen::destroyFont(font);
            }
            else {
                FL_CORE_ERROR("unabled to load font");
            }
            msdfgen::deinitializeFreetype(ft);
        }
        else {
            FL_CORE_ERROR("failed to initialize freetype");
        }
	}

    Font::~Font() {
        delete m_Data;
    }

    Ref<Font> Font::GetDefault() {
        static Ref<Font> DefaultFont;
        if (!DefaultFont) {
            DefaultFont = CreateRef<Font>("resources/Fonts/OpenSans/OpenSans-Regular.ttf");
        }
        return DefaultFont;
    }
}