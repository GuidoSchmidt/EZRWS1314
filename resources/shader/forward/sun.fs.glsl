//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsPosition;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;
layout (location = 1) out vec4 fragcolor2;


//*** Uniforms *****************************************************************
uniform sampler2D tex;
uniform vec3 color;


//*** Main *********************************************************************
void main(void)
{
    //fragcolor = texture(tex,vsUV);
    fragcolor2 = texture(tex,vsUV)*1.067-0.0669;
    //if (fragcolor.a<=0.5)
    //	discard;
    //fragcolor.rgb *= color;
    fragcolor2.rgb *= color;
}