#version 330 core

layout(location = 0) in vec2 aPos;

varying vec2 vTex;

uniform mat4 uMP;

void main()
{
	gl_Position = uMP * vec4(aPos.x, aPos.y, 0.0, 1);
	vTex = vec2(aPos.x, 1.0 - aPos.y);
}
