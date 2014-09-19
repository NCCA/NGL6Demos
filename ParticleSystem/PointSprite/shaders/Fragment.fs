#version 150
uniform sampler2D pointSprite;
out vec4 fragColour;
void main ()
{
  fragColour = texture(pointSprite,gl_PointCoord);
}
