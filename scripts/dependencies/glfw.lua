-- GLFW Dependencies
project "glfw"
    location (vendorProjectFiles)
	kind "StaticLib"
	language "C"
	staticruntime "off"

    targetdir (mainOutputdir)
    objdir (intermediateOutputdir)

	files
	{
		"%{wks.location}/Origin/vendor/glfw/include/GLFW/glfw3.h",
		"%{wks.location}/Origin/vendor/glfw/include/GLFW/glfw3native.h",
		"%{wks.location}/Origin/vendor/glfw/src/glfw_config.h",
		"%{wks.location}/Origin/vendor/glfw/src/context.c",
		"%{wks.location}/Origin/vendor/glfw/src/init.c",
		"%{wks.location}/Origin/vendor/glfw/src/input.c",
		"%{wks.location}/Origin/vendor/glfw/src/monitor.c",

		"%{wks.location}/Origin/vendor/glfw/src/null_init.c",
		"%{wks.location}/Origin/vendor/glfw/src/null_joystick.c",
		"%{wks.location}/Origin/vendor/glfw/src/null_monitor.c",
		"%{wks.location}/Origin/vendor/glfw/src/null_window.c",

		"%{wks.location}/Origin/vendor/glfw/src/platform.c",
		"%{wks.location}/Origin/vendor/glfw/src/vulkan.c",
		"%{wks.location}/Origin/vendor/glfw/src/window.c",
	}

	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"%{wks.location}/Origin/vendor/glfw/src/win32_init.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_joystick.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_module.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_monitor.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_time.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_thread.c",
			"%{wks.location}/Origin/vendor/glfw/src/win32_window.c",
			"%{wks.location}/Origin/vendor/glfw/src/wgl_context.c",
			"%{wks.location}/Origin/vendor/glfw/src/egl_context.c",
			"%{wks.location}/Origin/vendor/glfw/src/osmesa_context.c"
		}

		defines { 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"