#version 440 core

layout(vertices = 1) out;

void main(void)
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}