#version 330

#define nSamples 32

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCos;

uniform sampler2D maskSampler;

uniform ivec2 screenSize;
// uniform float nSamples;
uniform vec3 sunPos;


out vec2 radialUV1[nSamples/2];
out vec2 radialUV2[nSamples/2];
out vec2 UV;

float scale = -1.5;

vec2 nTexcoord(vec2 UV, int index, vec3 c) 
{
	float scale = sqrt(float(index)) * c.z + 1.0;
	return (UV - c.xy) * scale + c.xy;
}

void main() 
{
	gl_Position.xy = inPosition.xy;
	UV = inTexCos;
	vec3 ts = vec3(sunPos.x,sunPos.y, scale / nSamples);

	int j=0;
	for (int i = 0;i<nSamples/2;i++)
	{
		radialUV1[i]=nTexcoord(inTexCos,j,ts);
		j++;
		radialUV2[i]=nTexcoord(inTexCos,j,ts);
		j++;
	}
}