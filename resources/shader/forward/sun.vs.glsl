//VERTEX SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

//*** Output *******************************************************************
out vec3 vsPosition;
out vec2 vsUV;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	vsPosition = vec3( model * vec4(position, 1.0) );
	vsUV = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
}