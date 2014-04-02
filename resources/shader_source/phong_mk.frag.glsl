//FRAGMENT SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D diffuse_map;


//*** Functions ****************************************************************


//*** Main *********************************************************************
void main(void)
{
    // vec3 vsN = normalize(vsNormal);

    vec3 ambientColor = 1.0 * texture(diffuse_map, vsUV).rgb;

    fragcolor = vec4(ambientColor, 1.0);
}
