#version 150
/// @brief[in] the vertex normal
varying vec3 fragmentNormal;
/// @brief the vertex in eye co-ordinates non homogeneous
varying vec3 eyeCord3;
/// @brief the number of lights enabled
uniform int numLightsEnabled;
/// @brief the eye position passed in from main app
uniform vec3 eye;

/// @brief a function to compute point light values
/// @param[in] _light the number of the current light
/// @param[in] _normal the current fragmentNormal
/// @param[in,out] _ambient the ambient colour to be contributed to
/// @param[in,out] _diffuse the diffuse colour to be contributed to
/// @param[in,out] _specular the specular colour to be contributed to

void directionalLight(
												in int _light,
												in vec3 _normal,
												inout vec4 _ambient,
												inout vec4 _diffuse,
												inout vec4 _specular
											)
{
	/// @brief normal . light direction
	float nDotVP;
	/// @brief normal . half vector
	float nDotHV;
	/// @brief the power factor
	float powerFactor;
	// calculate the lambert term for the position vector
    nDotVP= max(0.0, dot(_normal,normalize(vec3 (gl_LightSource[_light].position))));
	// now see if we have any specular contribution
	if (nDotVP == 0.0)
	{
		powerFactor = 0.0; // no contribution
	}
	else
	{
		// and for the half vector for specular
		nDotHV= max(0.0, dot(_normal, vec3 (gl_LightSource[_light].halfVector)));

		// here we raise the shininess value to the power of the half vector
		// Phong / Blinn shading method
		powerFactor = pow(nDotHV, gl_FrontMaterial.shininess);
	}
	// finally add the lighting contributions using the material properties
	_ambient+=gl_FrontMaterial.ambient*gl_LightSource[_light].ambient;
	// diffuse is calculated by n.v * colour
	_diffuse+=gl_FrontMaterial.diffuse*gl_LightSource[_light].diffuse*nDotVP;
	// compute the specular value
	_specular+=gl_FrontMaterial.specular*gl_LightSource[_light].specular*powerFactor;
}



/// @brief a function to compute point light values
/// @param[in] _light the number of the current light
/// @param[in] _normal the current fragmentNormal
/// @param[in,out] _ambient the ambient colour to be contributed to
/// @param[in,out] _diffuse the diffuse colour to be contributed to
/// @param[in,out] _specular the specular colour to be contributed to

void pointLight(
								in int _light,
								in vec3 _normal,
								inout vec4 _ambient,
								inout vec4 _diffuse,
								inout vec4 _specular
							 )
{
	/// @brief normal . light direction
	float nDotVP;
	/// @brief normal . half vector
	float nDotHV;
	/// @brief the power factor
	float powerFactor;
	/// @brief the distance to the surface from the light
	float distance;
	/// @brief the attenuation of light with distance
	float attenuation;
	/// @brief the direction from the surface to the light position
	vec3 VP;
	/// @brief halfVector the direction of maximum highlights
	vec3 halfVector;

	/// compute vector from surface to light position

  VP=vec3(gl_LightSource[_light].position)-eyeCord3;
  // get the distance from surface to light
	distance=length(VP);
	VP=normalize(VP);
	// calculate attenuation of light through distance
	attenuation= 1.0 / (gl_LightSource[_light].constantAttenuation +
                      gl_LightSource[_light].linearAttenuation * distance +
                      gl_LightSource[_light].quadraticAttenuation * distance *distance);

	halfVector=normalize(VP+eye);
	// calculate the lambert term for the position vector
	nDotVP= max(0.0, dot(_normal,VP));
	// and for the half vector for specular
	nDotHV= max(0.0, dot(_normal, halfVector));

	// now see if we have any specular contribution
	if (nDotVP == 0.0)
	{
		powerFactor = 0.0; // no contribution
	}
	else
	{
		// here we raise the shininess value to the power of the half vector
		// Phong / Blinn shading method
		powerFactor = pow(nDotHV, gl_FrontMaterial.shininess);
	}
	// finally add the lighting contributions using the material properties
	_ambient+=gl_FrontMaterial.ambient*gl_LightSource[_light].ambient*attenuation;
	// diffuse is calculated by n.v * colour
	_diffuse+=gl_FrontMaterial.diffuse*gl_LightSource[_light].diffuse*nDotVP*attenuation;
	// compute the specular value
	_specular+=gl_FrontMaterial.specular*gl_LightSource[_light].specular*powerFactor*attenuation;

}



/// @brief a function to compute spotlight light values
/// @param[in] _light the number of the current light
/// @param[in] _eye the eye position
/// @param[in] _eyeCord3 the eye co-ordinate
/// @param[in] _normal the current fragmentNormal
/// @param[in,out] _ambient the ambient colour to be contributed to
/// @param[in,out] _diffuse the diffuse colour to be contributed to
/// @param[in,out] _specular the specular colour to be contributed to
const float cos_outer_cone_angle = 0.8; // 36 degrees
void spotLight(
								in int _light,
								in vec3 _eye,
								in vec3 _eyeCord3,
								in vec3 _normal,
								inout vec4 _ambient,
								inout vec4 _diffuse,
								inout vec4 _specular
							 )
{
	/// @brief normal . light direction
	float nDotVP;
	/// @brief normal . half vector
	float nDotHV;
	/// @brief the power factor
	float powerFactor;
	/// @brief if were in the spot or not
	float spotDot;
	/// @brief spot attenuation
	float spotAttenuation;
	/// @brief the distance to the surface from the light
	float distance;
	/// @brief the attenuation of light with distance
	float attenuation;
	/// @brief the direction from the surface to the light position
	vec3 VP;
	/// @brief halfVector the direction of maximum highlights
	vec3 halfVector;

 /// compute vector from surface to light position
    VP=vec3(gl_LightSource[_light].position)-_eyeCord3;
  // get the distance from surface to light
	distance=length(VP);

	// calculate attenuation of light through distance
	attenuation= 1.0 / (gl_LightSource[_light].constantAttenuation +
                      gl_LightSource[_light].linearAttenuation * distance +
                      gl_LightSource[_light].quadraticAttenuation * distance *distance);
	VP=normalize(VP);

	spotDot=dot(-VP, normalize(gl_LightSource[_light].spotDirection));

	if(spotDot < gl_LightSource[_light].spotCosCutoff)
	{
		spotAttenuation=0.0; // no effect for this light
	}
	else
	{

		spotAttenuation=pow(spotDot,gl_LightSource[_light].spotCosCutoff);
  }
	attenuation*=spotAttenuation;
  halfVector=normalize(VP-eye);
	// calculate the lambert term for the position vector
	nDotVP= max(0.0, dot(_normal,VP));
	// and for the half vector for specular
	nDotHV= max(0.0, dot(_normal, halfVector));

	// now see if we have any specular contribution
	if (nDotVP == 0.0)
	{
		powerFactor = 0.0; // no contribution
	}
	else
  {
		// here we raise the shininess value to the power of the half vector
		// Phong / Blinn shading method
		powerFactor = pow(nDotHV, gl_FrontMaterial.shininess);

	}
	// finally add the lighting contributions using the material properties
	_ambient+=gl_FrontMaterial.ambient*gl_LightSource[_light].ambient*attenuation;
	// diffuse is calculated by n.v * colour
	_diffuse+=gl_FrontMaterial.diffuse*gl_LightSource[_light].diffuse*nDotVP*attenuation;
	// compute the specular value
	_specular+=gl_FrontMaterial.specular*gl_LightSource[_light].specular*powerFactor*attenuation;
}

void main ()
{

	vec4 ambient=vec4(0.0);
	vec4 diffuse=vec4(0.0);
	vec4 specular=vec4(0.0);

	for (int i=0; i<numLightsEnabled; ++i)
	{
		if(gl_LightSource[i].position.w ==0.0)
		{
			directionalLight(i,fragmentNormal,ambient,diffuse,specular);
		}

	// if cutoff 180 it's not a spot so it must be a pointLight
		else if(gl_LightSource[i].spotCutoff== 180.0)
		{
			pointLight(i,fragmentNormal,ambient,diffuse,specular);
		}
		// if the light has a 0 for w then it's a vector so directional light
		else
		{
//			spotLight(i,eye,eyeCord3,fragmentNormal,ambient,diffuse,specular);
		}

	}
	gl_FragColor = ambient+diffuse+specular;

}
