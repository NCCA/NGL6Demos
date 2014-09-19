#version 150
/// @brief the current fragment normal for the vert being processed
out vec3 fragmentNormal;
/// @brief the vertex passed in
in vec3 inVert;
/// @brief the normal passed in
in vec3 inNormal;
/// @brief the in uv
in vec2 inUV;

// transform matrix values
uniform mat4 MVP;

void main(void)
{
// calculate the fragments surface normal
 fragmentNormal = inNormal;

 // calculate the vertex position
 gl_Position = MVP*vec4(inVert,1.0);

}






