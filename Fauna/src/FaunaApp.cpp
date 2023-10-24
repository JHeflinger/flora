#include <Flora.h>
#include <Flora/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Flora {
	class Fauna : public Application {
	public:
		Fauna(ApplicationCommandLineArgs args)
			: Application("Fauna", args) {
			PushLayer(new EditorLayer());
			GetWindow().SetWindowIcon("Resources/Icons/Editor/Logo.png");
		}

		~Fauna() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new Fauna(args);
	}
}