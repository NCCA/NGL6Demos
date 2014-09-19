#version 400 core
// some of this code is borrowed / modified from the
// Apple 	WWDC 2011 Instancing demo
// the view matrix for our object this will be
// created from the camera
uniform mat4 View;
// per draw data to modify our objects
uniform vec4 data;
// mouse rotatin passed in from our objet
uniform mat4 mouseRotation;
// this is the point position passed in
layout (location=0) in vec3 inPos;
// this matrix will be used as an output from our feedback buffer and fed
// into the next shader for per object transforms
out mat4 ModelView;
void main()
{
	//	Scale and spin each instance by a unique amount
	float spin = (gl_VertexID & 31) - 15.5;
	float c = cos(data.x * spin);
	float s = sin(data.x * spin);
	float y = (gl_VertexID & 15) * 0.125 + 0.25;
	float z = ((gl_VertexID + 5) & 63) * 0.03125 + 0.25;
	mat4 Model = mat4(   c, 0.0,  -s, 0.0,
												 0.0,   y, 0.0, 0.0,
													 s, 0.0, z*c, 0.0,
												 0.0, 0.0, 0.0, 1.0);
	// Translate each instance
	Model[3].xyz = inPos;
	// Rotate all instances around Z, scaled by dist
	float dist = length(inPos.xyz);
	float speed = data.y - dist * data.z + (gl_VertexID & 7) * 0.01;
	c = cos(data.x * speed);
	s = sin(data.x * speed);
	mat4 rotY = mat4(   c,   s, 0.0, 0.0,
												 -s,   c, 0.0, 0.0,
												0.0, 0.0, 1.0, 0.0,
												0.0, 0.0, 0.0, 1.0);
	Model = rotY * Model;
	Model = mouseRotation * Model;
	// Scale each instance
	Model[0] *= data.w;
	Model[1] *= data.w;
	Model[2] *= data.w;
	// Since we are only rendering from a single view, bake that in
	ModelView = View * Model;
}
