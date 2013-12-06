#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCos;

out vec2 varTexCos;

void main() 
{
	gl_Position.xy = inPosition.xy;
	varTexCos = inTexCos;
}