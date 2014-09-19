#version 400 core
/// @file Colour.fs
/// @brief a basic unshaded solid colour shader
/// @brief the colour to shade draw with
layout (location=0) out vec4 outColour;
uniform vec4 Colour;

void main ()
{
	outColour = Colour;
}

