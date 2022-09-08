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
      "%{IncludeDir.IMGUIZMO}/ImGuizmo.cpp",
      "%{IncludeDir.IMGUIZMO}/ImGuizmo.h",
	}

    includedirs {
        "%{wks.location}/Origin/sources",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.STBI}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.IMGUIZMO}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.ENTT}",
         "%{IncludeDir.YAML_CPP}",
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    links {
        "glfw",
        "glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:../../Origin/vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }

    

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