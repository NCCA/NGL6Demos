#version 400 core
// some of this code is borrowed / modified from the
// Apple 	WWDC 2011 Instancing demo

/// @brief MVP passed from app
uniform mat4 Projection;
// first attribute the vertex values from our VAO
layout (location =0) in vec3 inVert;
// second attribute the UV values from our VAO
layout(location =1) in vec2 inUV;
layout(location =2) in mat4 inModelView;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;
void main()
{
	mat4 ModelView = inModelView;
	mat4 ModelViewProjection = Projection * ModelView;
	// calculate the vertex position
	gl_Position = ModelViewProjection*vec4(inVert, 1.0);
	// pass the UV values to the frag shader
	vertUV=inUV.st;

}
