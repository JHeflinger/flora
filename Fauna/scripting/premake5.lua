local FloraRootDir = '../../'
include(FloraRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Penis"
architecture "x86_64"
startproject "Penis"

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

project "Penis"
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"

targetdir("Binaries")
objdir("Project Intermediates/Penis")

files
{
"D:/Dev/Penis/Assets/Scripts/**.cs", 
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

