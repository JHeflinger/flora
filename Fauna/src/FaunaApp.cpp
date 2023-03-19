#include <Flora.h>
#include <Flora/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Flora {
	class Fauna : public Application {
	public:
		Fauna()
			: Application("Fauna") {
			PushLayer(new EditorLayer());
		}

		~Fauna() {

		}
	};

	Application* CreateApplication() {
		return new Fauna();
	}
}