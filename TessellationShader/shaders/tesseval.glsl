#version 400
/*
layout( triangles, equal_spacing, ccw)  in;

void main( )
{
		vec4 p0 = gl_in[0].gl_Position;
		vec4 p1 = gl_in[1].gl_Position;
		vec4 p2 = gl_in[2].gl_Position;

		vec3 p = gl_TessCoord.xyz;

		gl_Position = p0*p.x + p1*p.y + p2*p.z;
}
*/

layout(triangles, equal_spacing, cw) in;
in vec3 tcPosition[];
out vec3 tePosition;
out vec3 tePatchDistance;
uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 MVP;

void main()
{
		vec3 p0 = gl_TessCoord.x * tcPosition[0];
		vec3 p1 = gl_TessCoord.y * tcPosition[1];
		vec3 p2 = gl_TessCoord.z * tcPosition[2];
		tePatchDistance = gl_TessCoord;
		tePosition = normalize(p0 + p1 + p2);
		gl_Position =   MVP * vec4(tePosition, 1);
}

