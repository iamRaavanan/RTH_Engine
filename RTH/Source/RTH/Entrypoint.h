#pragma once
#ifdef RTH_PLATFORM_WINDOWS

extern RTH::Application* RTH::CreateApplication();

void main(int argc, char** argv)
{
	printf("Entrypoint for RTH Engine!\n");
	auto app = RTH::CreateApplication();
	app->Run();
	delete app;
}

#endif