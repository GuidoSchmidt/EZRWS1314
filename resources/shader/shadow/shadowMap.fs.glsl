//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 ssPosition;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************

//*** Functions ****************************************************************

//*** Main *********************************************************************
void main(void)
{
    fragcolor = vec4(0.0, 0.0, 0.0 , 1.0);
}