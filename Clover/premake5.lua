project "Clover"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.hpp",
		"Source/**.cpp",
		"Source/**.cc",
		"Deps/stb_image/**.h",
		"Deps/stb_image/**.cpp",
		"Deps/ImGuizmo/**.h",
		"Deps/ImGuizmo/**.cpp",

		
		"Deps/imgui/examples/imgui_impl_glfw.cpp",
		"Deps/imgui/examples/imgui_impl_glfw.h",
		"Deps/imgui/examples/imgui_impl_opengl3.cpp",
		"Deps/imgui/examples/imgui_impl_opengl3.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{wks.location}/Clover/Source",
		"%{wks.location}/Clover/Deps/GLFW/include",
		"%{wks.location}/Clover/Deps/Glad/include",
		"%{wks.location}/Clover/Deps/imgui",
		"%{wks.location}/Clover/Deps/glm",
		"%{wks.location}/Clover/Deps/stb_image",
		"%{wks.location}/Clover/Deps/box2d/include",
		"%{wks.location}/Clover/Deps/yaml-cpp/include",
		"%{wks.location}/Clover/Deps/ImGuizmo",
	}

	links
	{
		"GLFW",
		"Box2D",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CL_WINDOWS_PLATFORM"
		}

	filter "configurations:Debug"
		defines "CF_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CF_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CF_DIST"
		runtime "Release"
		optimize "on"



