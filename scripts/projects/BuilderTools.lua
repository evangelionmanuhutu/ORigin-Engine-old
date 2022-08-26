-- Premake Project
project "BuilderTools"
    location "%{wks.location}/scripts"
    language "c++"
    kind "Utility"
    staticruntime "off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
      "%{wks.location}/scripts/build.bat",
      "%{wks.location}/scripts/Premake5.lua",
      "%{wks.location}/scripts/Projects/**.lua",
      "%{wks.location}/scripts/Dependencies/**.lua",
  	}