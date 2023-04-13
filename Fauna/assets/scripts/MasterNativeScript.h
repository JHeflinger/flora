#pragma once
#include "Flora/Scene/Components.h"
#include "BasicController.h"

namespace Flora {
	static void BindScriptToComponent(NativeScriptComponent& nsc, std::string typeName) {
		if (typeName == "BasicController")
			nsc.Bind<BasicController>();
		else FL_CORE_ASSERT(false, "Invalid Native Script");
	}
}