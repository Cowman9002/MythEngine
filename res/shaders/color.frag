#version 330 core

layout(location = 0) out vec4 fragColor;

uniform vec3 uColor = vec3(0.5, 0.5, 0.5);

void main()
{
	fragColor = vec4(uColor, 1.0);
}





