#version 150
/// @brief projection matrix passed in from camera class in main app
uniform mat4 projectionMatrix;
/// @brief View transform matrix passed in from camera class in main app
uniform mat4 ViewMatrix;
/// @brief Model transform matrix passed in from Transform Class in main app
uniform mat4 ModelMatrix;
/// @brief flag to indicate if model has unit normals if not normalize
uniform bool Normalize;
/// @brief flag to indicate if we are using texturing
uniform bool TextureEnabled;
varying vec3 fragmentNormal;
/// @brief the vertex in eye co-ordinates non homogeneous
varying vec3 eyeCord3;
/// @brief the number of lights enabled
uniform int numLightsEnabled;
/// @brief the eye position passed in from main app
uniform vec3 eye;

void main(void)
{
 // pre-calculate for speed we will use this a lot
 mat4 ModelView=ViewMatrix*ModelMatrix;
 // calculate the fragments surface normal
 fragmentNormal = (ModelView*vec4(gl_Normal, 0.0)).xyz;
 fragmentNormal*=gl_NormalMatrix;
 if (Normalize == true)
 {
  fragmentNormal = normalize(fragmentNormal);
 }

 // calculate the vertex position
 gl_Position = projectionMatrix*ModelView*gl_Vertex;
 // Transform the vertex to eye co-ordinates for frag shader
 /// @brief the vertex in eye co-ordinates  homogeneous
 vec4 eyeCord;
 eyeCord=ModelView*gl_Vertex;
 // divide by w for non homogenous
 eyeCord3=(vec3(eyeCord))/eyeCord.w;

 if (TextureEnabled == true)
 {
   gl_TexCoord[0] = gl_TextureMatrix[0]*gl_MultiTexCoord0;
 }

}
