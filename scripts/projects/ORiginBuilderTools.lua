-- Premake Project
project "ORiginBuilderTools"
    location "%{wks.location}/ORigin/%{prj.name}"
    language "C#"
    kind "ConsoleApp"
    staticruntime "off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
      "%{prj.location}/src/**.cs",
      "%{wks.location}/scripts/build.bat",
      "%{wks.location}/scripts/Premake5.lua",
      "%{wks.location}/scripts/Projects/**.lua",
      "%{wks.location}/scripts/Dependencies/**.lua",
  	}