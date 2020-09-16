#version 330 core

layout (location = 0) in vec3 aPos;

varying float vDepth;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPos, 1);
	vDepth = gl_Position.z;
}
