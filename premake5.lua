include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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

group "Dependencies"
	include "vendor/premake"
	include "Flora/vendor/Box2D"
	include "Flora/vendor/GLFW"
	include "Flora/vendor/Glad"
	include "Flora/vendor/imgui"
	include "Flora/vendor/yaml-cpp"
group ""

include "Flora"
include "Sandbox"
include "Fauna"
