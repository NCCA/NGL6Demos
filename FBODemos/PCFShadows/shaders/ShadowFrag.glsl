#version 400 core
/// modified from the OpenGL Shading Language Example "Orange Book"
/// Roost 2002
// this is needed on mac
//uniform sampler2DShadow ShadowMap;
// but under windows and linux this! don't know why!
uniform sampler2D ShadowMap;

flat in  vec2  ShadowScaleBias;
in  vec4  ShadowCoord;
in vec4 Colour;
layout (location = 0 ) out vec4 outColour;

void main()
{

	float sum;
	sum  = textureProjOffset(ShadowMap, ShadowCoord, ivec2(-1, -1)).x;
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2( 1, -1)).x;
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2(-1,  1)).x;
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2( 1,  1)).x;

	sum = sum * ShadowScaleBias.x + ShadowScaleBias.y;
	outColour = vec4(sum * 0.25 * Colour.rgb, Colour.a);
}
