workspace "TGP"
	architecture "x64"
	startproject "Modelviewer"

	configurations
	{
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "GraphicsEngine/"
include "ThirdParty/"
include "Modelviewer/"