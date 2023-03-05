project "Prototype"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{wks.location}/Prototype/src",
		"%{wks.location}/Clover/Source",
		"%{wks.location}/Clover/Deps/imgui",
		"%{wks.location}/Clover/Deps/glm",
		"%{wks.location}/Clover/Deps/stb_image",
		"%{wks.location}/Clover/Deps/yaml-cpp/include"
	}

	links
	{
		"Clover"
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



