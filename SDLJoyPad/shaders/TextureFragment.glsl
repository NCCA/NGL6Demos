#version 400

// this is a pointer to the current 2D texture object
uniform sampler2D tex;
// the vertex UV
in vec2 vertUV;
out vec4 outColour;
void main ()
{
 // set the fragment colour to the current texture
 outColour = texture(tex,vertUV);
}
