#version 400
layout (location=0) out vec4 fragColour;

/// @brief our output fragment colour
in vec4 perNormalColour;

void main ()
{
  fragColour = perNormalColour;
}

