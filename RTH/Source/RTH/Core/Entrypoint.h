#pragma once
#ifdef RTH_PLATFORM_WINDOWS

extern RTH::Application* RTH::CreateApplication();

void main(int argc, char** argv)
{
	RTH::Log::Init();
	RTH_CORE_WARN("Entrypoint for RTH Engine!");
	RTH_INFO("Welcome!\n");
	auto app = RTH::CreateApplication();
	app->Run();
	delete app;
}

#endif