#version 330

#define nSamples 16

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D maskSampler;

uniform ivec2 screenSize;
uniform float scale;
// uniform float nSamples;
uniform vec4 sunPos;

in vec2 UV;
in vec2 radialUV[nSamples];

vec3 l = vec3(0.2126, 0.7152, 0.0722); 

float nSamples_2 = nSamples/2.0;
float STEP=(1.0/nSamples);

//scale texcoord
vec2 nTexcoord (vec2 uv, float index, vec3 c) 
{
	float scale = sqrt(index) * c.z + 1.0;
	return (uv - c.xy) * scale + c.xy;
}

void main() 
{ 
	float lum = 1.0;
	colorOut = vec4(0,0,0,1);
	for (int i=0;i<nSamples/2;i++)
	{
		colorOut += texture(maskSampler,radialUV[i]) * lum;
		lum-=STEP;
		colorOut += texture(maskSampler,radialUV[i]) * lum;
		lum-=STEP;
	}
	colorOut*=0.25;
	colorOut=max(colorOut,0.0);
}