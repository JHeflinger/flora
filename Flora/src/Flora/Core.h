#pragma once

#ifdef FL_PLATFORM_WINDOWS
	#ifdef FL_BUILD_DLL
		#define FLORA_API __declspec(dllexport)
	#else
		#define FLORA_API __declspec(dllimport)
	#endif
#else
	#error Flora only supports windows!
#endif

#define BIT(x) (1 << x)