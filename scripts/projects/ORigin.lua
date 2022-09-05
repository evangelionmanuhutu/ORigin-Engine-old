-- Engine Project
project "ORigin"
    location (mainProjectFiles)
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    pchheader "pch.h"
    -- (pchsource) Under Windows, it must be a path relative to 'premake5.lua' (default premake file)
    pchsource "../../Origin/sources/pch.cpp" -- don't change this

    files {
	  "%{wks.location}/Origin/sources/**.h",
	  "%{wks.location}/Origin/sources/**.cpp",
	  "%{wks.location}/readme.md",
      "%{IncludeDir.STBI}/**.cpp",
      "%{IncludeDir.STBI}/**.h",
	}

    includedirs {
        "%{wks.location}/Origin/sources",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.STBI}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
         "%{IncludeDir.YAML_CPP}",
    }

    links {
        "glfw",
        "glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS",
        "YAMLCPP_USE_STATIC_LIBS"
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