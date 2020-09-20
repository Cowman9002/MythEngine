#version 330 core

#include std/pbr.glh

layout(location = 0) out vec4 fragColor;

varying vec2 vCoords;
varying vec3 vNorm;
varying vec3 vFragPos;
varying mat3 vTBN;

uniform sampler2D uTexture;
uniform sampler2D uRough;
uniform sampler2D uMetalness;
uniform sampler2D uNorm;
uniform vec3 uCamPos;

const vec3 L = normalize(vec3(-1.0, 1.0, 1.0));
const vec3 Radiance = vec3(1, 0.9, 0.6) * 2.0;

void main()
{
	float roughness = texture2D(uRough, vCoords).r;
	float alpha = roughness * roughness;

	float metalness = texture2D(uMetalness, vCoords).r;

	vec3 normal = texture2D(uNorm, vCoords).rgb;
	vec3 N = normal * 2.0 - 1.0;
	N = normalize(vTBN * N);

	vec3 V = normalize(uCamPos - vFragPos);
	vec3 H = normalize(L + V);
	vec3 iH = normalize(-L + V);
	vec3 R = normalize(reflect(-V, N));
	
	float NoV = abs(dot(N, V)) + 0.0001;
	float NoH = max(0.0, dot(N, H));
	float NoL = max(0.0, dot(N, L));
	float VoH = max(0.0, dot(V, H));
	float LoH = max(0.0, dot(L, H));
	
	vec3 main_color = texture2D(uTexture, vCoords).rgb;
	
	vec3 reflection = pow(vec3(0.5, 0.5, 0.58), vec3(2.2));
	vec3 radiance = reflection;
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, main_color, metalness);
	float D = GGX_NDF(NoH, alpha);
	float G = Smith_G(GGX_S(NoL, alpha), GGX_S(NoV, alpha));
	vec3 F = SchlickRough_F(F0, VoH, alpha);
	
	float omm = 1.0 - metalness;
	vec3 F2 = SchlickRough_F(F0, NoV, alpha);
	vec3 kD = 1.0 - F2;
	kD *= omm;
	
	vec3 specular = CookTorrance_BRDF(D, G, F, NoL, NoV);
	vec3 diffuse = Disney_D(main_color, NoV, NoL, LoH, roughness);
	//vec3 diffuse = Lambertian_D(main_color);
	vec3 ambient = kD * radiance * main_color + F2 * reflection;
	//vec3 ambient = kD * main_color;
	
	vec3 lighting = vec3(0.0);
	if(NoL > 0.0)
	{
		lighting = kD * diffuse + specular;
		lighting *= Radiance * NoL;
	}
	
	vec3 color = ambient + lighting;
	
	color = pow(color, vec3(1.0 / 2.2));
	
	fragColor = vec4(color, 1.0);
	//fragColor = vec3(irrad_sampling, 0.0);
}
