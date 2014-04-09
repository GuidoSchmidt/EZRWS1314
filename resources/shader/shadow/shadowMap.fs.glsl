//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
//in vec3 ssPosition;

//*** Output *******************************************************************
// layout (location = 0) out vec4 fragcolor;
layout (location = 0) out float fragDepth;

//*** Uniforms *****************************************************************

//*** Functions ****************************************************************

//*** Main *********************************************************************
void main(void)
{
    // fragcolor = vec4(vec3(gl_FragCoord.z), 1.0);
    //check texture for alpha
    fragDepth = gl_FragCoord.z;
}