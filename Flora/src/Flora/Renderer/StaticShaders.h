#pragma once
#include <string>

namespace Flora {
    class StaticShaders {
    public:
        static std::string TextureGLSL();
        static std::string CircleGLSL();
        static std::string LineGLSL();
        static std::string TextGLSL();
    };
}