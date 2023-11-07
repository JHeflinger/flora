#include "flpch.h"
#include "Font.h"
#undef INFINITE // <- man idk fix this later but its mainly in issue in windows
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"
#include "msdfgen.h"

namespace Flora {
    struct FontData {
        std::vector<msdf_atlas::GlyphGeometry> Glyphs;
        msdf_atlas::FontGeometry Geometry;
    };

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
                //37:10 https://www.youtube.com/watch?v=iMuiim9loOg

                if (false) {
                    msdfgen::Shape shape;
                    if (msdfgen::loadGlyph(shape, font, 'A')) {
                        shape.normalize();
                        //                      max. angle
                        msdfgen::edgeColoringSimple(shape, 3.0);
                        //           image width, height
                        msdfgen::Bitmap<float, 3> msdf(32, 32);
                        //                     range, scale, translation
                        msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
                        msdfgen::savePng(msdf, "output.png");
                    }
                }


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
}