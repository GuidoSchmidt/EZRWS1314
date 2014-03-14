//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsPosition;
in vec3 vsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform sampler2D day_tex;
uniform sampler2D night_tex;
uniform float blend;
uniform vec3 color;


//*** Main *********************************************************************
void main(void)
{
    fragcolor = mix(texture(night_tex,vsUV),texture(day_tex,vsUV),blend);
    fragcolor.rgb * color;
    fragcolor.a = 1;
    //fragcolor.rgb = vec3(1,0,0);
}