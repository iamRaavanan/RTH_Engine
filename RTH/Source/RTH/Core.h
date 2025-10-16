#pragma once
#ifdef RTH_PLATFORM_WINDOWS
	#ifdef RTH_BUILD_DLL
		#define RTH_API __declspec(dllexport)
	#else
		#define RTH_API __declspec(dllimport)
	#endif // RTH_BUILD_DLL
#else
#error RTH only supports Windows
#endif // RTH_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#ifdef RTH_ENABLE_ASSERTS
#define RTH_ASSERT(x, ...) {if(!(x)) {RTH_ERROR("Assertion Failed! {0}", __VA_ARGS__); __debugbreak();}}
#define RTH_CORE_ASSERT(x, ...) {if(!(x)) {RTH_CORE_ERROR("Assertion Failed! {0}", __VA_ARGS__); __debugbreak();}}
#else
#define RTH_ASSERT(x, ...)
#define RTH_CORE_ASSERT(x, ...)
#endif // RTH_ENABLE_ASSERTS
