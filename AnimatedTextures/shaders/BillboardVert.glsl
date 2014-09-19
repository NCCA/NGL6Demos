#version 400 core

layout (location = 0) in vec4 inVert;
layout (location =1) in float timeOffset;
flat out float whichTexture;
flat out float frameOffset;
void main()
{
    frameOffset=timeOffset;
    whichTexture=inVert.w;
    gl_Position = vec4(inVert.xyz, 1.0);
}
