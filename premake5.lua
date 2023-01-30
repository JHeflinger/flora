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

include "Flora/vendor/GLFW"
include "Flora/vendor/Glad"
include "Flora/vendor/imgui"

project "Flora"
	location "Flora"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "flpch.h"
	pchsource "Flora/src/flpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_WINDOWS",
			"FL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "FL_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"FL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "FL_DIST"
		runtime "Release"
		optimize "On"