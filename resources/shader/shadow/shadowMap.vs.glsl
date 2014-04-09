//VERTEX SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 uv;

//*** Output *******************************************************************
out vec2 UV;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	UV = uv;
	//ssPosition = vec3( projection * view * model * vec4(position, 1.0) );
	gl_Position = projection * view * model * vec4(position, 1.0);
}