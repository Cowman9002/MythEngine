#version 330 core

layout(location = 0) out vec4 fragColor;

uniform vec3 uColor = vec3(0.0, 1.0, 0.4);
uniform float uAlpha = 0.2;

mat4x4 thresholdMatrix =
{
	{1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0},
	{13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  7.0 / 17.0},
	{4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0},
	{16.0 / 17.0,  8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0}
};

void main()
{
	if(uAlpha - thresholdMatrix[int(gl_FragCoord.x) % 4][int(gl_FragCoord.y) % 4] < 0.0)
	{
		discard;
	}

	fragColor = vec4(uColor, 1.0);
}





