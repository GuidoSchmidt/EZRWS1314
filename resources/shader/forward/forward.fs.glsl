//FRAGMENT SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsPosition;
in vec3 vsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform vec3 diffuse_color;

//*** Main *********************************************************************
void main(void)
{
    fragcolor = vec4(diffuse_color, 1.0);
}