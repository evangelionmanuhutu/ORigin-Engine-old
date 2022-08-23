project "glad"
    location (vendorProjectFiles)
    kind "StaticLib"
	language "C"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    files {
        "%{wks.location}/Origin/vendor/glad/src/glad.c"
    }

    includedirs {
        "%{IncludeDir.GLAD}"
    }

    flags { 'NoPCH' }

    filter "system:windows"
        systemversion "latest"

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"