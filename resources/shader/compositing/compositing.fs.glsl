//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************
uniform lightBlock {
    vec3      lightPosition;
    sampler2D shadowMap;
    mat4 	  model;
    mat4 	  view;
    mat4 	  projection;
}

//*** Input ********************************************************************
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D shadowMap;
uniform sampler2D lightedMap;

//*** Global Variables *********************************************************
mat4 bias = mat4(0.5, 0.0, 0.0, 0.5,
			 	 0.0, 0.5, 0.0, 0.5,
			 	 0.0, 0.0, 0.5, 0.5,
			 	 0.0, 0.0, 0.0, 1.0);

//*** Functions ****************************************************************

//*** Main *********************************************************************
void main(void)
{
	mat4 shadowMapMatrix =  bias * projection * view * model;

	

    fragcolor = texture(lightedMap, vsUV);
}