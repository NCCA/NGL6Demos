#version 400 core
// this is base on http://http.developer.nvidia.com/GPUGems3/gpugems3_ch03.html
layout (location =0) in vec3 baseVert;
layout (location =1) in vec3 baseNormal;
layout (location =2) in vec3 poseVert1;
layout (location =3) in vec3 poseNormal1;
layout (location =4) in vec3 poseVert2;
layout (location =5) in vec3 poseNormal2;

// transform matrix values
uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 MV;
uniform float weight1;
uniform float weight2;
out vec3 position;
out vec3 normal;

void main()
{
	// first we computer the weighted normal
	vec3  finalN=baseNormal+(weight1*poseNormal1)+(weight2*poseNormal2);
	// then normalize and mult by normal matrix for shading
	normal = normalize( normalMatrix * finalN);
	// now calculate the eye cord position for the frag stage
	// now calculated the weighted vertices and add to the base mesh
	vec3  finalP=baseVert+(weight1*poseVert1)+(weight2*poseVert2);
	position = vec3(MV * vec4(finalP,1.0));
	// Convert position to clip coordinates and pass along
	gl_Position = MVP*vec4(finalP,1.0);
}









