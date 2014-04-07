#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D scene;
uniform sampler2D sky;
uniform sampler2D bloom;

uniform float bloomAmount;

in vec2 UV;

void main() 
{ 
	colorOut = texture(scene,UV)+texture(sky,UV)+bloomAmount*texture(bloom,UV);
}