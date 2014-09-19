#version 400
#pragma optionNV(unroll all)
layout(triangles) in;
layout(line_strip, max_vertices = 18) out;

in vec4 normal[];
in vec4 tangent[];
in vec4 binormal[];

uniform  float normalSize;
out vec4 perNormalColour;

void main()
{

		for(int i = 0; i<gl_in.length(); ++i)
		{
		gl_Position = gl_in[i].gl_Position;
		perNormalColour=vec4(1,0,0,1);
		EmitVertex();
		gl_Position = gl_in[i].gl_Position
									+ normal[i] * abs(normalSize);
		perNormalColour=vec4(1,0,0,1);
		EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[i].gl_Position;
		perNormalColour=vec4(0,1,0,1);
		EmitVertex();
		gl_Position = gl_in[i].gl_Position
									+ tangent[i] * abs(normalSize);
		perNormalColour=vec4(0,1,0,1);
		EmitVertex();
		EndPrimitive();


		gl_Position = gl_in[i].gl_Position;
		perNormalColour=vec4(0,0,1,1);
		EmitVertex();
		gl_Position = gl_in[i].gl_Position
									+ binormal[i] * abs(normalSize);
		perNormalColour=vec4(0,0,1,1);
		EmitVertex();
		EndPrimitive();


		}

}


