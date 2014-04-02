//VERTEX SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 uv;

//*** Output *******************************************************************
out vec3 Normal;
out vec2 vsUV;
out vec3 Position

//*** Uniforms *****************************************************************
uniform mat4 Model; // set
uniform mat4 View; // set
uniform mat4 Projection; // set
uniform mat4 MVP;
uniform mat3 NormalMatrix;


//*** Functions ****************************************************************


//*** Main *********************************************************************
void main(void)
{
    Normal = normalize( NormalMatrix * VertexNormal);

    // Vertex-Position without projection
    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

    // Vertex-Postion with projection
    gl_Position = MVP * vec4(VertexPosition,1.0);

}
