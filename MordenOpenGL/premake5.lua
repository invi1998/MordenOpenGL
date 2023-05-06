project "MordenOpenGL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "src/hzpch.h"
	pchsource "src/hzpch.cpp"
	

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

		-- 改为静态库后，就不需要将DLL拷贝到程序目录下了
		--postbuildcommands
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		--}
		filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"




