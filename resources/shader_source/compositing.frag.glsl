#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;
uniform sampler2D bloom;

uniform float bloomAmount;

in vec2 UV;

void main() 
{ 
	colorOut = texture(color,UV)+bloomAmount*texture(bloom,UV);
}