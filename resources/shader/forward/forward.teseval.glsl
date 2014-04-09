#version 440 core

layout(triangles, equal_spacing, ccw) in;

in vec3 tc_wsPosition[];

void main()
{
	vec3 p0 = gl_TessCoord.x * tc_wsPosition[0];	
	vec3 p1 = gl_TessCoord.y * tc_wsPosition[1];	
	vec3 p2 = gl_TessCoord.z * tc_wsPosition[2];	
	vec3 position = (p0 + p1 + p2);

	gl_Position = vec4(position, 1.0);
}