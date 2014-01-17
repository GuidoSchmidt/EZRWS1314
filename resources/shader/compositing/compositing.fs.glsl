//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D shadowMap;
uniform sampler2D lightedMap;

//*** Functions ****************************************************************

//*** Main *********************************************************************
void main(void)
{
    fragcolor = texture(shadowMap, vsUV) * texture(lightedMap, vsUV);
}