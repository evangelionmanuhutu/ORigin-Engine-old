project "ImGui"
    location (vendorProjectFiles)
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

    includedirs{
        "%{wks.location}/Origin/vendor/imgui/",
        "%{wks.location}/Origin/vendor/glfw/include",
    }

    files{
        "%{wks.location}/Origin/vendor/imgui/imconfig.h",
        "%{wks.location}/Origin/vendor/imgui/imgui.h",
        "%{wks.location}/Origin/vendor/imgui/imgui.cpp",
        "%{wks.location}/Origin/vendor/imgui/imgui_tables.cpp",
        "%{wks.location}/Origin/vendor/imgui/imgui_draw.cpp",
        "%{wks.location}/Origin/vendor/imgui/imgui_internal.h",
        "%{wks.location}/Origin/vendor/imgui/imgui_widgets.cpp",
        "%{wks.location}/Origin/vendor/imgui/imstb_rectpack.h",
        "%{wks.location}/Origin/vendor/imgui/imstb_textedit.h",
        "%{wks.location}/Origin/vendor/imgui/imstb_truetype.h",
        "%{wks.location}/Origin/vendor/imgui/imgui_demo.cpp",
    
        "%{wks.location}/Origin/vendor/imgui/backends/imgui_impl_glfw.cpp",
        "%{wks.location}/Origin/vendor/imgui/backends/imgui_impl_opengl3.cpp",
    }

    filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"