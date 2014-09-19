#version 400 core

/// @brief the vertex passed in
layout (location = 0) in vec3 inVert;
/// @brief the normal passed in
layout (location = 2) in vec3 inNormal;
/// @brief the in uv
layout (location = 1) in vec2 inUV;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform vec3 Colour;
out vec3 point;
out vec3 normal;
out vec3 colour;
//out vec3 zdepth;

void main ()
{
  point = ((MV * vec4 (inVert, 1.0)).xyz);
// may have to take into account normal matrix as well
  normal = normalize((MV * vec4 (inNormal, 0.0)).xyz);
  colour=Colour;
  gl_Position = MVP * vec4 (inVert, 1.0);
 // zdepth=vec3(gl_FragCoord.z);
}
