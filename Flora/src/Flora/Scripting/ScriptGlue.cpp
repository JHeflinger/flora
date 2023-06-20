#include "flpch.h"
#include "ScriptGlue.h"

namespace Flora {
	static void CoreTrace(MonoString* monostring) {
		char* string = mono_string_to_utf8(monostring);
		FL_CORE_TRACE(string);
		mono_free(string);
	}

	void ScriptGlue::RegisterFunctions() {
		//mono_add_internal_call
	}
}