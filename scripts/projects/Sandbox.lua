-- Sandbox Project
project "Sandbox"
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
		"%{wks.location}/Sandbox/sources/**.h",
		"%{wks.location}/Sandbox/sources/**.cpp",

        -- shader file
		"%{wks.location}/Sandbox/assets/shaders/**.glsl",
		"%{wks.location}/Sandbox/assets/shaders/**.hlsl",
        
        -- img file
        "%{wks.location}/Sandbox/assets/textures/**.png",
        "%{wks.location}/Sandbox/assets/textures/**.jpg",
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