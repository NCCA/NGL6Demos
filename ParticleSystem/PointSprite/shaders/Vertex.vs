#version 150
uniform mat4 MVP;

uniform sampler2D pointSprite;
/// @brief the vertex passed in
in vec4 inVert;

void main(void)
{
  gl_PointSize=clamp(inVert.w,0,24);
  vec4 pos=vec4(inVert.xyz,1);
  gl_Position = MVP*pos;
}
