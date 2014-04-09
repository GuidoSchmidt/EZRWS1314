#version 440 core

layout(triangles, invocations = 3) in;
layout (vertices=3) out;
out vec4 tc_wsPosition[];

void main(void)
{
	tc_wsPosition[gl_InvocationID] = gl_in[gl_InvocationID].gl_Position;
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 4.0;
		gl_TessLevelOuter[0] = 4.0;
		gl_TessLevelOuter[1] = 4.0;
		gl_TessLevelOuter[2] = 4.0;
	}
}