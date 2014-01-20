//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************
/*
uniform lightBlock {
    vec3      lightPosition;
    sampler2D shadowMap;
    mat4 	  model;
    mat4 	  view;
    mat4 	  projection;
}
*/

//*** Input ********************************************************************
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D shadowMap;
uniform sampler2D lightedMap;
uniform sampler2D positionMap;

mat4 lightmodel;
mat4 lightview;
mat4 lightprojection;

//*** Global Variables *********************************************************
mat4 bias = mat4(0.5, 0.0, 0.0, 0.5,
			 	 0.0, 0.5, 0.0, 0.5,
			 	 0.0, 0.0, 0.5, 0.5,
			 	 0.0, 0.0, 0.0, 1.0);

//*** Functions ****************************************************************
float linearize(in float value)
{
    float f = 100.0;
    float n = 0.1;
    float z = (2 * n) / (f + n - value * (f - n));
    return z;
}

//*** Main *********************************************************************
void main(void)
{	
    fragcolor = vec4( vec3( texture(shadowMap, vsUV).r ), 1.0 );
}