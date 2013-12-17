#version 330

#define nSamples 16

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCos;


uniform sampler2D maskSampler;

uniform ivec2 screenSize;
uniform float scale;
// uniform float nSamples;
uniform vec4 sunPos;


out vec2 radialUV[nSamples];
out vec2 UV;


vec2 nTexcoord(vec2 UV, int index, vec3 c) 
{
	float scale = sqrt(float(index)) * c.z + 1.0;
	return (UV - c.xy) * scale + c.xy;
}

void main() 
{
	gl_Position.xy = inPosition.xy;
	UV = inTexCos;
	vec3 ts = vec3(sunPos.x+0.5,sunPos.y+0.5, scale / nSamples);

	for (int i = 0;i<nSamples/2;i++)
	{
		radialUV[i]=nTexcoord(inTexCos,i,ts);
		i++;
		radialUV[i]=nTexcoord(inTexCos,i,ts);
	}
}