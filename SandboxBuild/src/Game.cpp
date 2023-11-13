#include <Flora.h>
#include <Flora/Core/EntryPoint.h>
#include "GameLayer.h"

namespace Flora {
	class Game : public Application {
	public:
		Game(const WindowProps& props, ApplicationCommandLineArgs args)
			: Application(props, "<INSERT GAME NAME HERE>", args) {
			PushLayer(new GameLayer());
			//GetWindow().SetWindowIcon("Resources/Icons/Editor/Logo.png");
		}

		~Game() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		WindowProps props;
		props.Title = "INSERT";
		props.Width = 1600;
		props.Height = 900;
		props.Fullscreen = false;
		return new Game(props, args);
	}
}