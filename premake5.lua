include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Clover"
	architecture "x86_64"
	startproject "Prototype"

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

group "Dependencies"
	include "Clover/Deps/Box2D"
	include "Clover/Deps/GLFW"
	include "Clover/Deps/Glad"
	include "Clover/Deps/imgui"
	include "Clover/Deps/yaml-cpp"
group ""

group "Engine"
	include "Clover"
group ""
	include "Prototype"