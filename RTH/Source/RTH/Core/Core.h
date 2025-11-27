#pragma once
#include <memory>

#ifdef RTH_PLATFORM_WINDOWS	
#if RTH_DYNAMIC_LINK
	#ifdef RTH_BUILD_DLL
		#define RTH_API __declspec(dllexport)
	#else
		#define RTH_API __declspec(dllimport)
	#endif // RTH_BUILD_DLL
#else
	#define RTH_API
#endif
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

#define RTH_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


namespace RTH
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}