project "GraphicsEngine"
	kind "StaticLib"
	language "C++"
	
	warnings "High"
	pchsource("gepch.cpp")
	pchheader "gepch.h"

	targetdir ("../libs/")
	targetname ("%{prj.name}-%{cfg.buildcfg}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	postbuildcommands 
	{
		"{COPY} ../ThirdParty/FBXImporter/lib/libfbxsdk.dll ../bin/"
	}

	files
	{
		"./**.h",
		"./**.hpp",
		"./**.cpp",
		"./**.hlsl",
		"./**.hlsli",
	}
	
	includedirs
	{
		"./",
		"../ThirdParty/"
	}
	
	libdirs
	{
		"../libs",
		"../ThirdParty/FBXImporter/lib"
	}

	links
	{
		"ThirdParty"
	}

	filter { "files:**.hlsl" }
		shadermodel "5.0"
		shaderobjectfileoutput "../Assets/Shaders/%{file.basename}.cso"

	filter { "files:**VS.hlsl" }
		shadertype "Vertex"

	filter { "files:**PS.hlsl" }
		shadertype "Pixel"
		
	filter { "files:**GS.hlsl" }
		shadertype "Geometry"

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "10.0"
		
		defines
		{
			"PLATFORM_WINDOWS",
			"NOMINMAX",
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
