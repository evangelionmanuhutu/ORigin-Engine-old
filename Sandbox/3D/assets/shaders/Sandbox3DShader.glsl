//type vertex
#version 460 core
layout(location = 0) in vec3 aPos;

out vec3 vPos;
uniform mat4 uTransform;

void main()
{
	vPos = aPos;
	gl_Position = uTransform * vec4(aPos, 1);
}

//type fragment
#version 460 core
in vec3 vPos;
out vec4 color;

uniform sampler2D u_Textures;

void main()
{
	color = vec4((vPos + 0.5), 1);
}
