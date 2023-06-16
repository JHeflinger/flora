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

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Flora/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows only
Library["WinSock"] = "Ws2_32.lib"
Library["Winmm"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["WinBCrypt"] = "Bcrypt.lib"