#version 400 core

/// modified from the OpenGL Shading Language Example "Orange Book"
/// Roost 2002

uniform mat3 normalMatrix;
uniform mat4 MV;
uniform mat4 MVP;
uniform mat4 textureMatrix;
uniform vec3 lightPosition;
uniform  vec4  inColour;

layout (location =0) in  vec4  inVert;
layout (location =1) in vec2 inUV;
layout (location =2) in  vec3  inNormal;
out vec4  ShadowCoord;
out vec4  Colour;
flat out  vec2  ShadowScaleBias;

void main()
{
	vec4 ecPosition = MV * inVert;
	vec3 ecPosition3 = (vec3(ecPosition)) / ecPosition.w;
	vec3 VP = lightPosition - ecPosition3;
	VP = normalize(VP);
	vec3 normal = normalize(normalMatrix * inNormal);
	float diffuse = max(0.0, dot(normal, VP));
	vec4 texCoord = textureMatrix * inVert;
	ShadowCoord   = texCoord;
	Colour  = vec4(diffuse * inColour.rgb, inColour.a);
	ShadowScaleBias.x=1;
	ShadowScaleBias.y=0.02;
	gl_Position    = MVP * inVert;
}

