workspace "RTH"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}%{cfg.system}%{cfg.architecture}"

project "RTH"
	location "RTH"
	kind "SharedLib"
	language "C++"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Plugins/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RTH_PLATFORM_WINDOWS",
			"RTH_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Binaries/" .. outputdir .. "/Playground")
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		optimize "On"
	
	filter "action:vs*"
		buildoptions { "/utf-8" }

		
project "Playground"
	location "Playground"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"RTH/Source",
		"RTH/Plugins/spdlog/include"
	}

	links
	{
		"RTH"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RTH_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		optimize "On"

	filter "action:vs*"
		buildoptions { "/utf-8" }