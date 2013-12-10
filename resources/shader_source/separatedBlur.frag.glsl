#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;
uniform float thresh;
uniform float range;
uniform float brightness;
uniform float horizontal;
uniform ivec2 screenSize;

in vec2 UV;


void main() 
{ 
	vec2 DELTA = vec2(1.0/float(screenSize.x),1.0/float(screenSize.y));
	vec4 sum=vec4(0);
	float n=0;
	vec2 dir = vec2(horizontal,(1.0-horizontal));
	//float i=10.0;
	for (float i=0.0;i<=range;i+=1.0)
	{
		sum+=texture(color,UV+dir*i*DELTA);
		sum+=texture(color,UV-dir*i*DELTA);
		n+=2.0;
	}
	colorOut = sum/n;
}