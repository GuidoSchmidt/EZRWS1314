#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCos;

out vec2 blurUV;
out vec2 maskUV;

vec3 sunPos = vec3(0.8,0.8,0.5);
//vec3 sunPos = vec3(-2,2,0.5);

void main() 
{
	gl_Position.xy = inPosition.xy;

	vec2 ts = sunPos.xy;

	blurUV = inTexCos;
	maskUV = blurUV - 0,5 * ts;
}