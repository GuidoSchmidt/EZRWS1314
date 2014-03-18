//VERTEX SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//*** Output *******************************************************************
out vec3 vsPosition;
smooth out vec3 vsNormal;
out vec2 vsUV;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	vsPosition = vec3( model * vec4(position, 1.0) );
	vsNormal = normalize(vec3(model * view * vec4(normal, 0.0) ));
	vsUV = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
}