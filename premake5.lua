include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "MordenOpenGL"
	architecture "x86_64"
	startproject "OpenGL"

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

group "Dependencies"
	include "vendor/premake"
	include "MordenOpenGL/vendor/GLFW"
	include "MordenOpenGL/vendor/Glad"
group ""


group "Core"
	include "MordenOpenGL"
group ""

group "APP"
	include "OpenGL"
group ""


