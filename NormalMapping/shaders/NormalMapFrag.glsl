#version 400
#pragma optionNV(unroll all)
// this is set for the base colour texture (unit 0)
uniform sampler2D tex;
// this is set to the spec map (texture unit 1)
uniform sampler2D spec;
// normal map set a texture unit 2
uniform sampler2D normalMap;
// array of light vectors passed from vert shader
in vec3 lightVec[3];
// array of half vects from vert shader
in vec3 halfVec[3];
// uv values passed from vert shader
in vec2 vertUV;
// spec power passed from client program (or could use alpha of spec map if present)
uniform float specPower;


// @brief light structure
struct Lights
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float spotCosCutoff;
	float spotCosInnerCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform Lights light[3];
in vec3 lightDir;
// out the blinn half vector
in vec3 halfVector;
in vec3 eyeVec;

in vec3 vPosition;

/// @brief our output fragment colour
out vec4 fragColour;

void main ()
{
fragColour=vec4(0);
// lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
vec3 normal=normalize( texture(normalMap, vertUV.st).xyz * 2.0 - 1.0);
// we need to flip the z as this is done in zBrush
normal.z = -normal.z;
// default material values to be accumulated
float lamberFactor;
vec4 diffuseMaterial = texture(tex, vertUV.st);
// compute specular lighting
vec4 specularMaterial=texture(spec, vertUV.st)  ;

float shininess ;


for (int i=0; i<3; ++i)
{
	lamberFactor= max (dot (lightVec[i], normal), 0.0) ;
	// so light is hitting use here calculate and accumulate values
	if (lamberFactor > 0.0)
	{
		// get the phong / blinn values
		shininess = pow (max (dot ( halfVec[i],normal), 0.0), specPower);
		fragColour +=diffuseMaterial * light[i].diffuse * lamberFactor;
		fragColour +=	specularMaterial * light[i].specular * shininess;
	}
}

}
