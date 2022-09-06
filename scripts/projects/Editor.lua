-- Sandbox Project
project "Editor"
    location (mainProjectFiles)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    links "ORigin"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
        -- source file
		"%{wks.location}/Editor/sources/**.h",
		"%{wks.location}/Editor/sources/**.cpp",

        -- shader file
		"%{wks.location}/Editor/assets/shaders/**.glsl",
		"%{wks.location}/Editor/assets/shaders/**.hlsl",
        
        -- img file
        "%{wks.location}/Editor/assets/textures/**.png",
        "%{wks.location}/Editor/assets/textures/**.jpg",
	}
    
    includedirs {
        "%{wks.location}/Origin/sources",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.YAML_CPP}",
    }

    defines {
        "YAMLCPP_USE_STATIC_LIBS",
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