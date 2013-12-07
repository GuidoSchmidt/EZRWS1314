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
out vec2 vsUV;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Main *********************************************************************
void main(void)
{   
	mat4 mvp   = projection * view * model;
    vsPosition = vec3( mvp * vec4(position, 1.0) );
    vsNormal   = vec3( transpose( inverse(view * model) ) * normalize( vec4(normal, 0.0) ) );
    vsUV = uv;

    gl_Position = vec4(vsPosition, 1.0);
}