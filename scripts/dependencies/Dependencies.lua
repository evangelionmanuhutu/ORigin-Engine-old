-- Set Dependencies Folder
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Origin/vendor/glfw/include"
IncludeDir["GLAD"] = "%{wks.location}/Origin/vendor/glad/include"
IncludeDir["IMGUI"] = "%{wks.location}/Origin/vendor/imgui"
IncludeDir["STBI"] = "%{wks.location}/Origin/vendor/stbi"
IncludeDir["SPDLOG"] = "%{wks.location}/Origin/vendor/spdlog/include"
IncludeDir["GLM"] = "%{wks.location}/Origin/vendor/glm"
IncludeDir["ENTT"] = "%{wks.location}/Origin/vendor/entt/"
IncludeDir["YAML_CPP"] = "%{wks.location}/Origin/vendor/yaml-cpp/include"

-- Dependencies Lua File
group "vendor"
include "glfw.lua"
include "glad.lua"
include "imgui.lua"
include "yaml-cpp.lua"
group ""