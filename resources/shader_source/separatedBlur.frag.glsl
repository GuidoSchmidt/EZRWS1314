#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;
uniform float thresh;
uniform float range;
uniform float brightness;
uniform float horizontal;
uniform vec2 screenSize;

in vec2 UV;


void main() {
	vec2 DELTA = vec2(1/float(screenSize.x),1/float(screenSize.y));
	vec3 sum;
	float n;
	vec2 dir = vec2(horizontal,(1-horizental));
	for (float i=0.0,i<=range,i+=1.0)
	{
		sum+=texture(color,UV+dir*i*DELTA).rgb;
		sum+=texture(color,UV-dir*i*DELTA).rgb;
		n+=2.0;
	}
	colorOut.rgb = sum/n;
}