-- Solution's Settings
workspace "ORigin"
    location "../"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    flags {

		"MultiProcessorCompile"
	}

    startproject "Editor"

    mainProjectFiles = "%{wks.location}/%{prj.name}"
    vendorProjectFiles = "%{wks.location}/Origin/vendor/projectFiles"

    mainOutputdir = "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    intermediateOutputdir = "%{wks.location}/bin/Intermediate/%{cfg.buildcfg}/%{prj.name}"

include "projects/ProjectSwitcher.lua"
include "dependencies/Dependencies.lua"

group "Engine"
include "projects/ORigin.lua"
include "projects/Editor.lua"
group ""

group"Programs"
include "projects/Sandbox.lua"
include "projects/BuilderTools.lua"
group ""