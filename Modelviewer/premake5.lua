project "Modelviewer"
	kind "WindowedApp"
	language "C++"
	
	targetdir ("../bin/")
	targetname ("%{prj.name}-%{cfg.buildcfg}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	
	files
	{
		"./**.h",
		"./**.hpp",
		"./**.cpp"
	}
	
	includedirs
	{
		"./",
		"../GraphicsEngine/",
		"../ThirdParty/",
	}
	
	libdirs
	{
		"../libs"
	}

	links
	{
		"GraphicsEngine"
	}

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
			"GraphicsEngine-Debug.lib",
		}
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
		links
		{
			"GraphicsEngine-Release.lib",
		}
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		
		links
		{
			"GraphicsEngine-Retail.lib",
		}