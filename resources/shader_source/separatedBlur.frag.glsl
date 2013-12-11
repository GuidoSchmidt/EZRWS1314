#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;
uniform float thresh;
//uniform float range;
uniform float brightness;
uniform float horizontal;
uniform ivec2 screenSize;

float range = 10.0;
in vec2 UV;
vec3 l = vec3(0.2126, 0.7152, 0.0722); 

float lum(vec4 col)
{
	return dot(l,col.rgb);
}

void main() 
{ 
	vec2 DELTA = vec2(1.0/float(screenSize.x),1.0/float(screenSize.y));
	vec4 sum=vec4(0.0);
	float n=0.0;

	//center Tap
	vec4 tap = texture(color,UV);
	float brightEnough = max(0.0,sign(lum(tap)-thresh));
	sum+=tap;
	n+=1;

	//separated direction
	vec2 dir = vec2(horizontal,(1.0-horizontal));
	//neighbours
	for (float i=1.0;i<=range;i+=1.0)
	{
		/*
		vec4 tap = texture(color,UV+dir*i*DELTA);
		if (lum(tap)>thresh) 
		{
			sum+=tap;
			n+=1.0;
		}

		tap = texture(color,UV-dir*i*DELTA);
		if (lum(tap)>thresh) 
		{
			sum+=tap;
			n+=1.0;
		}*/

		//ohne if abfragen
		tap = texture(color,UV+dir*i*DELTA);
		brightEnough = max(0.0,sign(lum(tap)-thresh));
		sum+=brightEnough*tap;
		n+=brightEnough;

		tap = texture(color,UV-dir*i*DELTA);
		brightEnough = max(0.0,sign(lum(tap)-thresh));
		sum+=brightEnough*tap;
		n+=brightEnough;
	}
	colorOut = sum/n;
}