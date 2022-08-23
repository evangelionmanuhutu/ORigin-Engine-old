-- Engine Project
project "ORigin"
    location (mainProjectFiles)
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    pchheader "pch.h"
    -- (pchsource) Under Windows, it must be a path relative to 'premake5.lua' (default premake file)
    pchsource "../../ORigin/sources/pch.cpp" -- don't change this

    files {
	  "%{wks.location}/ORigin/sources/**.h",
	  "%{wks.location}/ORigin/sources/**.cpp",
	  "%{wks.location}/readme.md",
      "%{IncludeDir.STBI}/**.cpp",
      "%{IncludeDir.STBI}/**.h",
      "%{IncludeDir.ENTT}/**.hpp",
	}

    includedirs {
        "%{wks.location}/ORigin/sources",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.STBI}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
    }

    links {
        "glfw",
        "glad",
        "ImGui",
        "opengl32.lib"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		defines "ORG_DEBUG"
        runtime "Debug"
		symbols "On"
        
        filter "configurations:Release"
		defines "ORG_RELEASE"
        runtime "Release"
		optimize "On"
        
        filter "configurations:Dist"
		defines "ORG_DIST"
        runtime "Release"
		optimize "On"