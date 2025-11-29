#pragma once
#ifdef RTH_PLATFORM_WINDOWS

extern RTH::Application* RTH::CreateApplication();

void main(int argc, char** argv)
{
	RTH::Log::Init();

	RTH_PROFILE_BEGIN_SESSION("Startup", "RTH-Startup.json");
	auto app = RTH::CreateApplication();
	RTH_PROFILE_END_SESSION();

	RTH_PROFILE_BEGIN_SESSION("Runtime", "RTH-Runtime.json");
	app->Run();
	RTH_PROFILE_END_SESSION();

	RTH_PROFILE_BEGIN_SESSION("Shutdown", "RTH-Shutdown.json");
	delete app;
	RTH_PROFILE_END_SESSION();
}
	
#endif