#version 400 core

/// @file Colour.vs
/// @brief a basic unshaded solid colour shader used with Colour.fs

/// @brief MVP matrix passed from our app
uniform mat4 MVP;

// first attribute the vertex values from our VAO
layout (location=0) in vec3 inVert;
uniform vec4 Colour;

void main()
{
// calculate the vertex position
gl_Position = MVP*vec4(inVert, 1.0);
}
