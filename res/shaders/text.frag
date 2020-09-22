#version 330 core

layout(location = 0) out vec4 fragColor;

varying vec2 vTex;

uniform sampler2D uCharacter;
uniform vec3 uColor = vec3(1.0);

void main()
{
	fragColor = vec4(uColor, texture2D(uCharacter, vTex).r);
	//fragColor = vec4(vec3(texture2D(uCharacter, vTex).r), 1.0);
}





