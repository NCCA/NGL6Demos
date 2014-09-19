#version 400

/// @brief Model transform matrix passed in from Transform Class in main app
uniform mat4 MVP;

layout (location=0) in vec3 inVert;
layout (location=1)in vec2 inUV;
out vec2 vertUV;
uniform int xMultiplyer=1;
uniform float yOffset=0;

void main()
{
 // calculate the vertex position
 gl_Position = MVP*vec4(inVert, 1.0);
 // get the texture co-ord and mutliply it by the texture matrix
 vertUV=inUV.st;
 vertUV.s *=xMultiplyer;
 vertUV.t-=yOffset;
}
