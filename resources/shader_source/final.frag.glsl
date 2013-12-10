#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;

uniform ivec2 screenSize;

in vec2 UV;

void main() 
{ 
	colorOut = texture(color,UV);
//	colorOut = vec4(1,0,0,1);
}