#version 400

// this is a pointer to the current 2D texture object
uniform sampler2D tex;
// the vertex UV
in vec2 vertUV;
layout (location =0) out vec4 fragColour;

void main ()
{
 // set the fragment colour to the current texture
 fragColour = vec4(texture(tex,vertUV));
}
