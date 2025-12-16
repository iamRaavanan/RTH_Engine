workspace "RTH"
	architecture "x64"
	startproject "RTH-Editor"
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
IncludeDir["stb_image"] = "RTH/Plugins/stb_image"

group "Dependencies"
	include "RTH/Plugins/GLFW"
	include "RTH/Plugins/Glad"
	include "RTH/Plugins/Imgui"
group ""

project "RTH"
	location "RTH"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "Rthpch.h"
	pchsource "%{prj.name}/Source/Rthpch.cpp"
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Plugins/stb_image/**.cpp",
		"%{prj.name}/Plugins/stb_image/**.h",
		"%{prj.name}/Plugins/glm/glm/**.hpp",
		"%{prj.name}/Plugins/glm/glm/**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/Plugins/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}
	links
	{
		"GLFW",
		"Glad",
		"Imgui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"IMGUI_API=__declspec(dllexport)",
			"RTH_PLATFORM_WINDOWS",
			"RTH_BUILD_DLL",
			"RTH_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		-- postbuildcommands
		-- {
		-- 	("{COPYFILE} %{cfg.buildtarget.relpath} %{wks.location}/Binaries/" .. outputdir .. "/Playground")
		-- }
	
	filter "configurations:Debug"
		defines "RTH_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RTH_DIST"
		runtime "Release"
		optimize "on"
	
	filter "action:vs*"
		buildoptions { "/utf-8" }

		
project "Playground"
	location "Playground"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		symbols "on"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RTH_DIST"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
		buildoptions { "/utf-8" }
		
project "RTH-Editor"
	location "RTH-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		symbols "on"

	filter "configurations:Release"
		defines "RTH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RTH_DIST"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
		buildoptions { "/utf-8" }