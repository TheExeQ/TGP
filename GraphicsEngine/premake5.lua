project "GraphicsEngine"
	kind "StaticLib"
	language "C++"
	
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
		"./**.hlsl"
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
	
	filter { "files:**.hlsl" }
		shadermodel "5.0"

	filter { "files:**-vs.hlsl" }
		shadertype "Vertex"

	filter { "files:**-ps.hlsl" }
		shadertype "Pixel"

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
		
		links
		{
			"d3d11.lib",
			"TGAFBXImporterd.lib"
		}
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
		links
		{
			"d3d11.lib",
			"TGAFBXImporter.lib"
		}
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		
		links
		{
			"d3d11.lib",
			"TGAFBXImporter.lib"
		}