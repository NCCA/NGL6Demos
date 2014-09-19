#version 400
#pragma optionNV(unroll all)
// first attribute the vertex values from our VAO
layout (location =0) in vec3 inVert;
// second attribute the UV values from our VAO
layout (location =1) in vec2 inUV;
// third attribute the  normals values from our VAO
layout (location =2) in vec3 inNormal;
// forth attribute the Tangents values from our VAO
layout (location =3) in vec3 inTangent;
// fith attribute the binormal values from our VAO
layout (location =4) in vec3 inBinormal;

struct Lights
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float spotCosCutoff;
	float spotCosInnerCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

// array of lights
uniform Lights light[3];
// direction of the lights used for shading
out vec3 lightDir;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 MV;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;
// an array of light Vectors to be passed to the frag shader
out vec3 lightVec[3];
// an array of half (blinn) vectors to be passed to the frag shader
out vec3 halfVec[3];

void main()
{

	// calculate the vertex position
	gl_Position = MVP*vec4(inVert, 1.0);
	// pass the UV values to the frag shader
	vertUV=inUV.st;


	vec4 worldPosition = MV * vec4(inVert, 1.0);

	// now fill the array of light pos and half vectors for the avaliable lights
	for (int i=0; i<3; ++i)
	{
	vec3 lightDir = normalize(light[i].position.xyz - worldPosition.xyz);
	// transform light and half angle vectors by tangent basis
	// this is based on code from here
	//http://www.ozone3d.net/tutorials/bump_mapping.php
	// as our values are already normalized we don't need to here
	lightVec[i].x = dot (lightDir, inTangent );
	lightVec[i].y = dot (lightDir, inBinormal);
	lightVec[i].z = dot (lightDir, inNormal);
	vec3 halfVector = normalize(worldPosition.xyz + lightDir);
	halfVec[i].x = dot (halfVector, inTangent);
	halfVec[i].y = dot (halfVector, inBinormal);
	halfVec[i].z = dot (halfVector, inNormal);
	}

}
