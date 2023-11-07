#include "flpch.h"
#include "Font.h"
#undef INFINITE // <- man idk fix this later but its mainly in issue in windows
#include "msdf-atlas-gen.h"
#include "msdfgen.h"

namespace Flora {
	Font::Font(const std::filesystem::path& filepath) {
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
        if (ft) {
            msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath.string().c_str());
            if (font) {
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
                msdfgen::destroyFont(font);
            }
            msdfgen::deinitializeFreetype(ft);
        }
	}
}