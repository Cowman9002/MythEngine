#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNorm;

varying vec3 vNorm;
varying vec2 vTexcoord;

uniform mat4 uMVP;
uniform mat4 uM;

void main()
{
	gl_Position = uMVP * vec4(aPos, 1);
	vNorm = mat3(transpose(inverse(uM))) * aNorm;
	vTexcoord = aTex;
}
