#version 400 core

uniform sampler2D pointTex;
uniform sampler2D normalTex;
uniform sampler2D colourTex;
uniform sampler2D lightPassTex;
uniform sampler2D diffuseTex;

in vec2 vertUV;
layout (location =0) out vec4 fragColour;
uniform int mode;
uniform vec3 cam;


void main()
{

  vec4 point=texture(pointTex,vertUV);
  vec4 normal=texture(normalTex,vertUV);
  vec4 colour=texture(colourTex,vertUV);
  vec4 final=texture(lightPassTex,vertUV);
  vec4 diffuse=texture(diffuseTex,vertUV);
  switch(mode)
  {
    case 0:
        fragColour=colour;//colour*max(dot(normal,point),0.0);
    break;
  case 1:
      fragColour=point;//colour*max(dot(normal,point),0.0);
  break;
  case 2:
      fragColour=normalize(normal);//colour*max(dot(normal,point),0.0);
  break;

  case 3:

//    vec3 light = vec3(2,2,2);
//    vec3 lightDir = light - point.xyz ;

//    normal = normalize(normal);
//    lightDir = normalize(lightDir);

//    vec3 eyeDir = normalize(cam-point.xyz);
//    vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);
//      fragColour.rgb = max(dot(normal.xyz,lightDir),0) * colour.rgb +
//                     pow(max(dot(normal.xyz,vHalfVector),0.0), 3) * 1.5;
      fragColour=diffuse;
    break;

   case 4 :
        vec3 light = vec3(2,2,2);
        vec3 lightDir = light - point.xyz ;

        normal = normalize(normal);
        lightDir = normalize(lightDir);

        vec3 eyeDir = normalize(cam-point.xyz);
        vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);
          fragColour.rgb = max(dot(normal.xyz,lightDir),0) * colour.rgb +
                         pow(max(dot(normal.xyz,vHalfVector),0.0), 3) * 1.5;
    break;

  }
}
