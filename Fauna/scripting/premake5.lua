local FloraRootDir = '../../'
include(FloraRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Flora of Life"
architecture "x86_64"
startproject "Flora of Life"

configurations
{
"Debug",
"Release",
"Dist"
}

flags
{
"MultiProcessorCompile"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Flora of Life"
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"

targetdir("Binaries")
objdir("Project Intermediates/Flora of Life")

files
{
"D:/Dev/FOL/Assets/Scripts/**.cs", 
}

links
{
"Flora-ScriptCore"
}

filter "configurations:Debug"
optimize "Off"
symbols "Default"

filter "configurations:Release"
optimize "On"
symbols "Default"

filter "configurations:Dist"
optimize "Full"
symbols "Off"

group "Flora"
include(FloraRootDir .. "/Flora-ScriptCore")
group ""

