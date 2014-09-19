#version 400 core

in vec3 point;
in vec3 normal;
in vec3 colour;
in vec3 zdepth;
layout (location=0)out vec3 pointDeferred;
layout (location=1)out vec3 normalDeferred;
layout (location=2)out vec3 colourDeferred;
layout (location=3)out vec3 shadingDeferred;
uniform vec3 cam;

void main ()
{
  pointDeferred = point;
  normalDeferred = normal;
  colourDeferred= colour;

//  float ndcDepth =
//      (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
//      (gl_DepthRange.far - gl_DepthRange.near);
  vec3 light = vec3(2,2,2);
  vec3 lightDir = light - point.xyz ;

  vec3 N = normalize(normal);
  lightDir = normalize(lightDir);

  vec3 eyeDir = normalize(cam-point.xyz);
  vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);
    shadingDeferred = max(dot(N,lightDir),0) * colour.rgb +
                   pow(max(dot(N,vHalfVector),0.0), 3) * 1.5;

  //lightPassTex=vec3(gl_FragCoord.z * 2.0 - 1.0 );



}
