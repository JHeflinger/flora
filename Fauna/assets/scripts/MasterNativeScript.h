#pragma once
#include "Flora/Scene/Components.h"
#include "BasicController.h"
#include "AutoRotater.h"
#include "TestScript.h"

namespace Flora {
	static void BindScriptToComponent(NativeScriptComponent& nsc, std::string typeName) {
		if (typeName == "BasicController")
			nsc.Bind<BasicController>();
		else if (typeName == "AutoRotater")
			nsc.Bind<AutoRotater>();
		else if (typeName == "TestScript")
			nsc.Bind<TestScript>();
		else FL_CORE_ASSERT(false, "Invalid Native Script");
	}
}