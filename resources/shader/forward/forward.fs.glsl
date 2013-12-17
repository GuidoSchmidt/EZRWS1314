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
uniform sampler2D uv_test_texture;
uniform float sky;

//*** Main *********************************************************************
void main(void)
{
    //fragcolor = mix( vec4(0.5, 0.5, 0.5, 1.0), texture(uv_test_texture, vsUV), 0.0 );
    fragcolor = texture(uv_test_texture, vsUV);
    if (fragcolor.a<=0.5)
    	discard;
    if (sky==1.0)
    	fragcolor=fragcolor/(1-fragcolor);
    fragcolor.a = sky;
}