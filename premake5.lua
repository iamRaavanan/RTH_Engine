workspace "RTH"
	architecture "x64"
	startproject "Playground"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}%{cfg.system}%{cfg.architecture}"

-- Including the directories relative to the root folder
IncludeDir = {}
IncludeDir["GLFW"] = "RTH/Plugins/GLFW/include"
IncludeDir["Glad"] = "RTH/Plugins/Glad/include"
IncludeDir["Imgui"] = "RTH/Plugins/imgui"
IncludeDir["glm"] = "RTH/Plugins/glm"
include "RTH/Plugins/GLFW"
include "RTH/Plugins/Glad"
include "RTH/Plugins/Imgui"

project "RTH"
	location "RTH"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "Rthpch.h"
	pchsource "%{prj.name}/Source/Rthpch.cpp"
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Plugins/glm/glm/**.hpp",
		"%{prj.name}/Plugins/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Plugins/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Imgui}",
		"%{IncludeDir.glm}"
	}
	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"IMGUI_API=__declspec(dllexport)",
			"RTH_PLATFORM_WINDOWS",
			"RTH_BUILD_DLL",
			"RTH_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} %{wks.location}/Binaries/" .. outputdir .. "/Playground")
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		runtime "Release"
		optimize "On"
	
	filter "action:vs*"
		buildoptions { "/utf-8" }

		
project "Playground"
	location "Playground"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"RTH/Plugins/spdlog/include",
		"RTH/Plugins/imgui",
		"%{IncludeDir.glm}"
	}

	links
	{
		"RTH"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"IMGUI_API=__declspec(dllimport)",
			"RTH_PLATFORM_WINDOWS",
			"RTH_ENABLE_ASSERTS"
		}
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "RTH_DIST"
		buildoptions "/MD"
		optimize "On"

	filter "action:vs*"
		buildoptions { "/utf-8" }