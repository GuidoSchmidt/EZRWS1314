#version 440 core

layout(vertices = 3) out;
out vec4 tc_wsPosition[];

in vec3 wsPosition[];
in vec2 vsUV[];

out vec3 tcPosition[];
out vec2 tcTexCoord[];

uniform float TessLevelInner;
uniform float TessLevelOuter;

#define ID gl_InvocationID

void main(void)
{
	tcPosition[ID] = wsPosition[ID];
	tcTexCoord[ID] = vsUV[ID];

	if(ID == 0)
	{
		gl_TessLevelInner[0] = 4.0;
		gl_TessLevelOuter[0] = 8.0;
		gl_TessLevelOuter[1] = 8.0;
		gl_TessLevelOuter[2] = 8.0;
	}
}