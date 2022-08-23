-- Set Dependencies Folder
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ORigin/vendor/glfw/include"
IncludeDir["GLAD"] = "%{wks.location}/ORigin/vendor/glad/include"
IncludeDir["IMGUI"] = "%{wks.location}/ORigin/vendor/imgui"
IncludeDir["STBI"] = "%{wks.location}/ORigin/vendor/stbi"
IncludeDir["SPDLOG"] = "%{wks.location}/ORigin/vendor/spdlog/include"
IncludeDir["GLM"] = "%{wks.location}/ORigin/vendor/glm"
IncludeDir["ENTT"] = "%{wks.location}/ORigin/vendor/entt/"

-- Dependencies Lua File
group "Vendor"
include "glfw.lua"
include "glad.lua"
include "imgui.lua"
group ""