#version 330 core

layout(location = 0) out vec4 fragColor;

varying float vDepth;

const float NEAR_MUL = 1.0;
const float FAR_MUL = 0.1;

const float FAR = 30.0;

uniform vec3 uColor = vec3(0.0, 1.0, 0.4);

void main()
{
	fragColor = vec4(uColor * mix(NEAR_MUL, FAR_MUL, clamp(vDepth / FAR, 0.0, 1.0)), 1.0);
}





