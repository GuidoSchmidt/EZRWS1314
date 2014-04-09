//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec2 UV;

//*** Output *******************************************************************
// layout (location = 0) out vec4 fragcolor;
layout (location = 0) out float fragDepth;

//*** Uniforms *****************************************************************

uniform sampler2D diffuse_tex;
//*** Functions ****************************************************************

//*** Main *********************************************************************
void main(void)
{
    if (texture(diffuse_tex,UV).a < 0.2)
    	discard;
    fragDepth = gl_FragCoord.z;
}