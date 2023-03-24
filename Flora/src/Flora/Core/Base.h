#pragma once
#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define FL_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR == 1
			#error "IOS simulator is not supported!"
		#elif TARGET_OS_IPHONE == 1
			#define FL_PLATFORM_IOS
			#error "IOS is not supported!"
		#elif TARGET_OS_MAC == 1
			#define FL_PLATFORM_MACOS
			#error "MacOS is not supported!"
		#else
			#error "Unknown Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define FL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define FL_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif

// DLL support
#ifdef FL_PLATFORM_WINDOWS
	#if FL_DYNAMIC_LINK
		#ifdef FL_BUILD_DLL
			#define FLORA_API __declspec(dllexport)
		#else
			#define FLORA_API __declspec(dllimport)
		#endif
	#else
		#define FLORA_API
	#endif
#else
	#error Flora only supports Windows!
#endif

//#ifdef FL_DEBUG
	//#define FL_ENABLE_ASSERTS
//#endif

#ifdef FL_ENABLE_ASSERTS
	#define FL_ASSERT(x, ...) { if(!(x)) {FL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define FL_CORE_ASSERT(x, ...) { if(!(x)) { FL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); debugbreak(); }}
#else
	#define FL_ASSERT(x, ...)
	#define FL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Flora {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}