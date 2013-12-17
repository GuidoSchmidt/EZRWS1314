#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D color;
//uniform float thresh;
//uniform float range;
uniform float brightness;
uniform float horizontal;
uniform ivec2 screenSize;

float range = 5.0;
in vec2 UV;
//// vec3 l = vec3(0.2126, 0.7152, 0.0722); 
vec3 l = vec3(1.0/3.0); 
//float thresh = 0.5;

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
	vec4 that = texture(color,UV);
	//float isBrightEnough = max(0.0,sign(lum(tap)-thresh));
	//sum+=tap;
	//n+=1;

	//separated direction
	vec2 dir = vec2(horizontal,(1.0-horizontal));
	//dir = vec2(dir.y,dir.x);
	//neighbours
	//colorOut = vec4(lum(tap));
	//colorOut.rgb = vec3(that);
	//colorOut = that;
	vec4 tap = texture(color,UV+dir*2.0*DELTA);
	sum += tap*1*tap.a;
	tap = texture(color,UV-dir*1.0*DELTA);
	sum += tap*2*tap.a;
	tap = texture(color,UV+dir*0.0*DELTA);
	sum += tap*3*tap.a;
	tap = texture(color,UV+dir*1.0*DELTA);
	sum += tap*2*tap.a;
	tap = texture(color,UV-dir*2.0*DELTA);
	sum += tap*1*tap.a;

	// for (float i=-range;i<=range;i+=1.0)
	// {
	// 	float weight = 1/(abs(i)+1);
	// 	vec4 tap = texture(color,UV+dir*i*DELTA) * weight;
	// 	if (tap.a == 1.0)
	// 	{
	// 		sum+=tap;
	// 		n+=weight;
	// 	}
	// }
	colorOut = sum/9;//3.9;
	//colorOut = vec4(that.a);
	colorOut.a=that.a;
}