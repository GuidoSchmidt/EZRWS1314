//VERTEX SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;

//*** Output *******************************************************************
//out vec3 ssPosition;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	//ssPosition = vec3( projection * view * model * vec4(position, 1.0) );
	gl_Position = projection * view * model * vec4(position, 1.0);
}