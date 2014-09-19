#version 400 core
/// @brief our output fragment colour
layout (location =0) out vec4 fragColour;

// this is a pointer to the current 2D texture object
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
// the vertex UV
in vec2 vertUV;
flat in float texID;

void main ()
{

 // set the fragment colour to the current texture
	if(texID==0)
	{
            fragColour = texture(tex1,vertUV);
	}
	else if(texID==1)
	{
            fragColour = texture(tex2,vertUV);
	}
	else
	{
            fragColour = texture(tex3,vertUV);
	}

 if (fragColour.r == 0 && fragColour.g == 0 && fragColour.b == 0)
 {
   discard;
 }
 //fragColour=vec4(1);
}
