#version 400 core

uniform sampler2D pointTex;
uniform sampler2D normalTex;
uniform sampler2D colourTex;
uniform vec3 lightPos;
uniform mat4 V;
in vec2 vertUV;
layout (location =0) out vec4 fragColour;
const vec3 kd = vec3 (1.0, 1.0, 1.0);
const vec3 ks = vec3 (1.0, 1.0, 1.0);
const float specular_exponent = 100.0;
const vec3 ld = vec3(1.0,1.0,1.0);
const vec3 ls = vec3(1.0,1.0,1.0);


vec3 phong (in vec3 p_eye, in vec3 n_eye, in vec3 colour)
{
  vec3 light_position_eye = vec3 (V * vec4 (lightPos, 1.0));
  vec3 dist_to_light_eye = light_position_eye - p_eye;
  vec3 direction_to_light_eye = normalize (dist_to_light_eye);

  // standard diffuse light
  float dot_prod = max (dot (direction_to_light_eye,  n_eye), 0.0);
  vec3 Id = ld * kd * dot_prod; // final diffuse intensity
  /*
  // standard specular light
  vec3 reflection_eye = reflect (-direction_to_light_eye, n_eye);
  vec3 surface_to_viewer_eye = normalize (-p_eye);
  float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
  dot_prod_specular = max (dot_prod_specular, 0.0);
  float specular_factor = pow (dot_prod_specular, specular_exponent);
  vec3 Is = ls * ks * specular_factor; // final specular intensity

  // attenuation (fade out to sphere edges)
  float dist_2d = distance (light_position_eye, p_eye);
  float atten_factor = -log (min (1.0, dist_2d / 5.0));

  return (Id + Is) * atten_factor;
  */
  return Id*colour;
}



void main()
{
  vec2 st;
   st.s = gl_FragCoord.x / 1024.0/2.0;
   st.t = gl_FragCoord.y / 720.0/2.0;

  vec4 point=texture(pointTex,st);
  vec4 normal=texture(normalTex,st);
  vec4 colour=texture(colourTex,st);

  // skip background
   if (point.z > -0.0001) {
     discard;
   }

fragColour.rgb = phong(point.rgb, normalize (normal).rgb,colour.rgb);
// fragColour=colour;//colour*max(dot(normal,point),0.0);
fragColour.a=1.0;
///fragColour=point;
}
