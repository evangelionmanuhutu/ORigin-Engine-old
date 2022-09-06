project "yaml-cpp"
    location (vendorProjectFiles)
	kind "StaticLib"
	language "C++"
	staticruntime "Off"

	targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

	files {
        "%{wks.location}/Origin/vendor/yaml-cpp/src/**.cpp",
        "%{wks.location}/Origin/vendor/yaml-cpp/src/**.h",
        "%{wks.location}/Origin/vendor/yaml-cpp/include/yaml-cpp/**.h",
    }

	defines {
        "YAMLCPP_USE_STATIC_LIBS"
    }

    includedirs {
        "%{wks.location}/Origin/vendor/yaml-cpp/include/"
    }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"