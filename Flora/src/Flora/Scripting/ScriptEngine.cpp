#include "flpch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Flora {
	struct ScriptEngineData {

	};

	static ScriptEngineData s_Data;

	void ScriptEngine::Init() {
		InitMono();
	}

	void ScriptEngine::Shutdown() {

	}

	void ScriptEngine::InitMono() {
		mono_set_assemblies_path("mono/lib");
	}
}