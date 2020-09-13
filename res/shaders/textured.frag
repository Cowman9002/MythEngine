#version 330 core

layout(location = 0) out vec4 fragColor;

varying vec3 vNorm;
varying vec2 vTexcoord;

uniform sampler2D uTexture;
uniform vec3 uColorMix = vec3(1.0);

const vec3 L = normalize(vec3(-1.0, 1.0, 1.0));

void main()
{
	vec3 N = normalize(vNorm);

	vec3 color = texture2D(uTexture, vTexcoord).rgb * uColorMix * 0.02;
	
	color += color * max(0.0, dot(N, L)) * 20.0;

	color = color / (color + vec3(1));

	color = pow(color, vec3(1.0 / 2.2));

	fragColor = vec4(color, 1.0);
}





