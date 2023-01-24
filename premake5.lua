workspace "Flora"
    architecture "x64"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Flora"
    location "Flora"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "flpch.h"
    pchsource "Flora/src/flpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "FL_PLATFORM_WINDOWS",
            "FL_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "FL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FL_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FL_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "Consoleapp"
    language "C++"

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
        "Flora/src"
    }

    links
    {
        "Flora"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "FL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "FL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FL_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FL_DIST"
        optimize "On"