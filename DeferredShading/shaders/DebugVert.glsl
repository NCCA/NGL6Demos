#version 400 core

// first attribute the vertex values from our VAO
layout (location = 0) in vec3 inVert;
// second attribute the UV values from our VAO
layout (location = 1) in vec2 inUV;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;
void main()
{
// pre-calculate for speed we will use this a lot

    // calculate the vertex position
    gl_Position = vec4(inVert, 1.0);
    // pass the UV values to the frag shader
    vertUV=inUV.st;
}
