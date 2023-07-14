IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Flora/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Flora/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/Flora/vendor/box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/Flora/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Flora/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Flora/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Flora/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Flora/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Flora/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/Flora/vendor/mono/include"
IncludeDir["filewatch"] = "%{wks.location}/Flora/vendor/filewatch"
IncludeDir["openAL"] = "%{wks.location}/Flora/vendor/openAL/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Flora/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["openAL"] = "%{wks.location}/Flora/vendor/openAL/libs/Win64"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows only
Library["WinSock"] = "Ws2_32.lib"
Library["Winmm"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["WinBCrypt"] = "Bcrypt.lib"

-- OpenAL: note that we're just gonna use the win32 build
Library["openAL"] = "%{LibraryDir.openAL}/OpenAL32.lib"
Library["efx"] = "%{LibraryDir.openAL}/EFX-Util_MT/EFX-Util.lib"