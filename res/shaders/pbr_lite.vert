#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNorm;
layout(location = 3) in vec3 aTang;

varying vec2 vCoords;
varying vec3 vFragPos;
varying mat3 vTBN;

uniform mat4 uMVP;
uniform mat4 uModelMat;

void main()
{
	gl_Position = uMVP * vec4(aPos, 1.0);
	vFragPos = (uModelMat * vec4(aPos, 1.0)).xyz;
	vCoords = aTex;
	
	vec3 T = normalize(vec3(uModelMat * vec4(aTang, 0.0)));
	vec3 N = normalize(vec3(uModelMat * vec4(aNorm, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));
	vTBN = mat3(T, B, N);
	
	//vNorm = uNormMat * aNorm;
	//vNorm = aNorm;
}
