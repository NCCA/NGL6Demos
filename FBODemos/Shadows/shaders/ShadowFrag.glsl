#version 400 core

/// modified from the OpenGL Shading Language Example "Orange Book"
/// Roost 2002

in vec4 Colour;
layout (location=0) out vec4 outColour;
// for mac we need this
//uniform sampler2DShadow ShadowMap;
// for windows unix we need
uniform sampler2D ShadowMap;

in vec4 ShadowCoord;


void main ()
{
	float shadeFactor=textureProj(ShadowMap,ShadowCoord).x;
        shadeFactor=shadeFactor;// * 0.25 + 0.75;
	outColour=vec4(shadeFactor * Colour.rgb, Colour.a);
	//outColour=vec4(ShadowCoord);
}
