#version 150
uniform sampler2D tex;

/// @brief MVP passed from app
// first attribute the vertex values from our VAO
in vec3 inVert;
// second attribute the UV values from our VAO
in vec2 inUV;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;
uniform mat4 MVP;

void main(void)
{
// pre-calculate for speed we will use this a lot

// calculate the vertex position
gl_Position = MVP*vec4(inVert, 1.0);
// pass the UV values to the frag shader
vertUV=inUV.st;
}
