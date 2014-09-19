#version 400
//layout( vertices = 3 )  out;
//in vec3 vPosition[];
//out vec3 tcPosition[];

//void main( )
//{
//    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
//    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

//    gl_TessLevelOuter[0] = 1.0;
//    gl_TessLevelOuter[1] = 1.0;
//    gl_TessLevelOuter[2] = 1.0;
//    gl_TessLevelInner[0] = 1.0;
//    gl_TessLevelInner[1] = 1.0;
//}

layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tcPosition[];
uniform float TessLevelInner;
uniform float TessLevelOuter;


void main()
{
		tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
		if (gl_InvocationID == 0)
		{
				gl_TessLevelInner[0] = TessLevelInner;
				gl_TessLevelOuter[0] = TessLevelOuter;
				gl_TessLevelOuter[1] = TessLevelOuter;
				gl_TessLevelOuter[2] = TessLevelOuter;
		}
}
