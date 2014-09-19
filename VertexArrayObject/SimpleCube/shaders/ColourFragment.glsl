#version 400 core
in vec3 vertColour;
layout (location=0)out vec4 outColour;
void main ()
{
 // set the fragment colour
 outColour = vec4(vertColour,1);
}
