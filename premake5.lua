workspace "Flora"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Flora/vendor/GLFW/include"
IncludeDir["Glad"] = "Flora/vendor/Glad/include"
IncludeDir["ImGui"] = "Flora/vendor/imgui"
IncludeDir["glm"] = "Flora/vendor/glm"
IncludeDir["stb_image"] = "Flora/vendor/stb_image"

include "Flora/vendor/GLFW"
include "Flora/vendor/Glad"
include "Flora/vendor/imgui"

project "Flora"
	location "Flora"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "flpch.h"
	pchsource "Flora/src/flpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Flora/vendor/spdlog/include",
		"Flora/src",
		"%{IncludeDir.glm}",
		"Flora/vendor"
	}

	links
	{
		"Flora"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FL_DIST"
		runtime "Release"
		optimize "on"