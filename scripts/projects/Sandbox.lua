
-- Sandbox2D Project
project "Sandbox2D"
    location "%{wks.location}/Sandbox/2D"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "ORigin"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
        -- source file
		"%{wks.location}/Sandbox/2D/sources/**.h",
		"%{wks.location}/Sandbox/2D/sources/**.cpp",

        -- shader file
		"%{wks.location}/Sandbox/2D/assets/shaders/**.glsl",
		"%{wks.location}/Sandbox/2D/assets/shaders/**.hlsl",
        
        -- img file
        "%{wks.location}/Sandbox/2D/assets/textures/**.png",
        "%{wks.location}/Sandbox/2D/assets/textures/**.jpg",
	}
    
    includedirs {
        "%{wks.location}/Origin/sources",
        "%{wks.location}/Sandbox/2D/sources",
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

-- Sandbox3D Project
project "Sandbox3D"
    location "%{wks.location}/Sandbox/3D"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "ORigin"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
        -- source file
		"%{wks.location}/Sandbox/3D/sources/**.h",
		"%{wks.location}/Sandbox/3D/sources/**.cpp",

        -- shader file
		"%{wks.location}/Sandbox/3D/assets/shaders/**.glsl",
		"%{wks.location}/Sandbox/3D/assets/shaders/**.hlsl",
        
        -- img file
        "%{wks.location}/Sandbox/3D/assets/textures/**.png",
        "%{wks.location}/Sandbox/3D/assets/textures/**.jpg",
	}
    
    includedirs {
        "%{wks.location}/Origin/sources",
        "%{wks.location}/Sandbox/3Dsources",
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