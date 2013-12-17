#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCos;

//uniform scale;
float scale = 2;
out vec2 blurUV;
out vec2 maskUV;
out vec2 sunUV;

uniform vec3 sunPos;// = vec3(0.8,0.8,0.5);
//vec3 sunPos = vec3(-2,2,0.5);

void main() 
{
	gl_Position.xy = inPosition.xy;

	//sunUV = sunPos.xy/sunPos.z);///2+0.5;

	vec2 scaleV = vec2(scale,scale);
	blurUV = inTexCos;
	maskUV = (blurUV-sunPos.xy)/scale+0.5;
	// maskUV = ((blurUV+0.5)/scaleV)-sunUV*scaleV-0.5;
}