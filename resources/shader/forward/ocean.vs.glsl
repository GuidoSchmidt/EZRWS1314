//VERTEX SHADER
#version 400

//--- Uniform block definitions ------------------------------------------------

//--- Input --------------------------------------------------------------------
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//--- Output -------------------------------------------------------------------
out vec3 vPosition;

//--- Uniforms -----------------------------------------------------------------

//--- Main ---------------------------------------------------------------------
void main(void)
{ 
	vPosition = position.xyz;
}