local FloraRootDir = '../../../..'
include(FloraRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Tower of Icarus"
architecture "x86_64"
startproject "Tower of Icarus"

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

project "Tower of Icarus"
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"

targetdir("Binaries")
objdir("Project Intermediates/Tower of Icarus")

files
{
"D:/Dev/ToI/Assets/Scripts/**.cs", 
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

