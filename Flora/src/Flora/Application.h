#pragma once
#include "Core.h"

namespace Flora {
	class FLORA_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

