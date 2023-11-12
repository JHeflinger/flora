#pragma once
#include <vector>
#undef INFINITE // <- man idk fix this later but its mainly in issue in windows
#include "msdf-atlas-gen.h"

namespace Flora {
    struct FontData {
        std::vector<msdf_atlas::GlyphGeometry> Glyphs;
        msdf_atlas::FontGeometry Geometry;
    };
}