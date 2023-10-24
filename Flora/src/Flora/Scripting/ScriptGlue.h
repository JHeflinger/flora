#pragma once
#include "Flora/Scene/Components.h"

namespace Flora {
	class ScriptGlue {
	public:
		static void RegisterFunctions();
		static void RegisterComponents();
	private:
		template<typename... Component>
		static void RegisterComponent();

		template<typename... Component>
		static void RegisterComponent(ComponentGroup<Component ...>);
	};
}