#version 400
#pragma optionNV(unroll all)
precision highp float;
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

uniform mat4 MVP;

uniform float normalSize;

out vec4 normal;
out vec4 tangent;
out vec4 binormal;

uniform bool drawFaceNormals;
uniform bool drawVertexNormals;

void main()
{
	gl_Position = MVP*vec4(inVert,1);

	normal=MVP*vec4(inNormal,0);
	tangent=MVP*vec4(inTangent,0);
	binormal=MVP*vec4(inBinormal,0);
}
