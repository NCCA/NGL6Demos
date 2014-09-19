#version 400 core
// this is base on http://http.developer.nvidia.com/GPUGems3/gpugems3_ch03.html
layout (location =0) in vec3 baseVert;
layout (location =1) in vec3 baseNormal;

// transform matrix values
uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 MV;
uniform float weight1;
uniform float weight2;
out vec3 position;
out vec3 normal;
uniform samplerBuffer TBO;
void main()
{
	// so the data is passed in a packed array, we have vec4's with
	// vert1 vert2 normal1 normal2 so we offset vertex Id by 4 and then index in
	// to get our correct value
	vec3 poseVert1=texelFetch(TBO,int(4*gl_VertexID)).xyz;
	vec3 poseVert2=texelFetch(TBO,int(4*gl_VertexID+1)).xyz;
	vec3 poseNormal1=texelFetch(TBO,int(4*gl_VertexID+2)).xyz;
	vec3 poseNormal2=texelFetch(TBO,int(4*gl_VertexID+3)).xyz;

	vec3  finalN=baseNormal+(weight1*poseNormal1)+(weight2*poseNormal2);
	// then normalize and mult by normal matrix for shading
	normal = normalize( normalMatrix * finalN);
	// now calculate the eye cord position for the frag stage
	position = vec3(MV * vec4(baseVert,1.0));
	// now calculated the weighted vertices and add to the base mesh
	vec3  finalP=baseVert+(weight1*poseVert1)+(weight2*poseVert2);
	// Convert position to clip coordinates and pass along
	gl_Position = MVP*vec4(finalP,1.0);

}









