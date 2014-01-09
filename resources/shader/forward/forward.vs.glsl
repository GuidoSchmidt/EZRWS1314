//VERTEX SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//*** Output *******************************************************************
out vec3 vsPosition;
out vec3 vsNormal;
out vec3 wsNormal;
out vec2 vsUV;
out vec3 wsPosition;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	vsPosition = vec3( model * vec4(position, 1.0) );
	wsPosition = position;
	vsNormal = normalize(vec3(model * view * vec4(normal, 0.0) ));
	vsUV = uv;
	wsNormal = normalize(normal);
	gl_Position = projection * view * model * vec4(position, 1.0);
}