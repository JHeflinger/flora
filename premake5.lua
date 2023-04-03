include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Flora"
	architecture "x86_64"
	startproject "Fauna"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Flora/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Flora/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Flora/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Flora/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Flora/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Flora/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Flora/vendor/yaml-cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "Flora/vendor/GLFW"
	include "Flora/vendor/Glad"
	include "Flora/vendor/imgui"
	include "Flora/vendor/yaml-cpp"
group ""

include "Flora"
include "Sandbox"
include "Fauna"
