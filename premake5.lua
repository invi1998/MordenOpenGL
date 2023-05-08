include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "MordenOpenGL"
	architecture "x86_64"
	startproject "MordenOpenGL"

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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dev"
	include "vendor/premake"
	include "MordenOpenGL"
	include "MordenOpenGL/vendor/GLFW"
	include "MordenOpenGL/vendor/Glad"
group ""
