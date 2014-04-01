//VERTEX SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//*** Output *******************************************************************
out vec3 vsNormal;
out vec2 vsUV;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//*** Functions ****************************************************************


//*** Main *********************************************************************
void main(void)
{
//     vsNormal = vec3(normalize(transpose(inverse(model * view)) *
//                    vec4(normal, 0.0)));

//     passing uv coordinates to fragment shader
    vsUV = uv;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
