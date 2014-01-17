//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************
uniform lightBlock {
    vec3      lightPosition;
    sampler2D shadowMap;
}

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
    fragcolor = texture(lightedMap, vsUV);
}