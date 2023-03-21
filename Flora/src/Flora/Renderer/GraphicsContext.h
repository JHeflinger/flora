#pragma once

namespace Flora {
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}