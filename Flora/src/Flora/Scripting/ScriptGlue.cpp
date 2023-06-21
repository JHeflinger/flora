#include "flpch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"

namespace Flora {
	
	#define FL_ADD_INTERNAL_CALL(funcName) mono_add_internal_call("Flora.InternalCalls::" #funcName, funcName)

	static void CoreTrace(MonoString* monostring) {
		char* string = mono_string_to_utf8(monostring);
		FL_CORE_TRACE(string);
		mono_free(string);
	}

	void ScriptGlue::RegisterFunctions() {
		FL_ADD_INTERNAL_CALL(CoreTrace);
	}
}