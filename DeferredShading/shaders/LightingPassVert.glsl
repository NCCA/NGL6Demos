#version 400 core
// first attribute the vertex values from our VAO
layout (location = 0) in vec3 inVert;
uniform mat4 MVP;
void main()
{
// pre-calculate for speed we will use this a lot

    // calculate the vertex position
    gl_Position = MVP*vec4(inVert, 1.0);
}
