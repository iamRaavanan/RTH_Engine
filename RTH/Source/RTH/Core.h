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