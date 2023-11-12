#include <Flora.h>
#include <Flora/Core/EntryPoint.h>
#include "GameLayer.h"

namespace Flora {
	class Game : public Application {
	public:
		Game(ApplicationCommandLineArgs args)
			: Application("<INSERT GAME NAME HERE>", args) {
			PushLayer(new GameLayer());
			//GetWindow().SetWindowIcon("Resources/Icons/Editor/Logo.png");
		}

		~Game() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new Game(args);
	}
}