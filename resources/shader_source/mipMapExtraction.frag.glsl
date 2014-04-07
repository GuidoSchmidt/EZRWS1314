#version 330

layout(location = 0) out vec4 minAveMax;
 
uniform sampler2D inputTexture;

uniform ivec2 screenSize;
uniform float firstPass;

in vec2 UV;

//vec3 l = vec3(0.2126, 0.7152, 0.0722); 
vec3 l = vec3(0.3333); 

float lum(vec4 col)
{
	return dot(l,col.rgb);
}

void main() 
{	
	vec2 DELTA = vec2(1.0/screenSize.x,1.0/screenSize.y);
	vec2 offset;

	if (firstPass == 1.0)
	{
		vec4 taps;

		// offset = vec2(0.0, 		  0.0);//0.5*DELTA;
		// taps.x = lum(texture(inputTexture,UV+offset));
		// offset = vec2(0.0*DELTA.x,0.5*DELTA.y);//0.5*DELTA;
		// taps.y = lum(texture(inputTexture,UV+offset));
		// offset = vec2(0.5*DELTA.x,0.0*DELTA.y);//0.5*DELTA;
		// taps.z = lum(texture(inputTexture,UV+offset));
		// offset = vec2(0.5*DELTA.x,0.5*DELTA.y);//0.5*DELTA;
		// taps.w = lum(texture(inputTexture,UV+offset));

		offset = vec2( 0.25*DELTA.x, 0.25*DELTA.y);//0.5*DELTA;
		taps.x = lum(texture(inputTexture,UV+offset));
		offset = vec2(-0.25*DELTA.x, 0.25*DELTA.y);//0.5*DELTA;
		taps.y = lum(texture(inputTexture,UV+offset));
		offset = vec2( 0.25*DELTA.x,-0.25*DELTA.y);//0.5*DELTA;
		taps.z = lum(texture(inputTexture,UV+offset));
		offset = vec2(-0.25*DELTA.x,-0.25*DELTA.y);//0.5*DELTA;
		taps.w = lum(texture(inputTexture,UV+offset));


		minAveMax.x = min(min(min(taps.x,taps.y),taps.z),taps.w); 
		minAveMax.y = dot(taps,vec4(0.25));
		minAveMax.z = max(max(max(taps.x,taps.y),taps.z),taps.w); 

		minAveMax.w=1.0;
	}
	else
	{
		vec4 tap1,tap2,tap3,tap4;

		// offset = vec2( 0.0*DELTA.x, 0.0*DELTA.y);//-0.5*DELTA;
		// tap1 = texture(inputTexture,UV+offset);
		// offset = vec2(0.0*DELTA.x, 0.5*DELTA.y);//-0.5*DELTA;
		// tap2 = texture(inputTexture,UV+offset);
		// offset = vec2( 0.5*DELTA.x,0.5*DELTA.y);//-0.5*DELTA;
		// tap3 = texture(inputTexture,UV+offset);
		// offset = vec2(0.5*DELTA.x,0.5*DELTA.y);//-0.5*DELTA;
		// tap4 = texture(inputTexture,UV+offset);
	
		offset = vec2( 0.25*DELTA.x, 0.25*DELTA.y);//-0.5*DELTA;
		tap1 = texture(inputTexture,UV+offset);
		offset = vec2(-0.25*DELTA.x, 0.25*DELTA.y);//-0.5*DELTA;
		tap2 = texture(inputTexture,UV+offset);
		offset = vec2( 0.25*DELTA.x,-0.25*DELTA.y);//-0.5*DELTA;
		tap3 = texture(inputTexture,UV+offset);
		offset = vec2(-0.25*DELTA.x,-0.25*DELTA.y);//-0.5*DELTA;
		tap4 = texture(inputTexture,UV+offset);

		minAveMax.x = min(min(min(tap1.x,tap2.x),tap3.x),tap4.x); 
		minAveMax.y = dot(vec4(tap1.y,tap2.y,tap3.y,tap4.y),vec4(0.25));
		minAveMax.z = max(max(max(tap1.z,tap2.z),tap3.z),tap4.z); 

		minAveMax.w=1.0;
	}
}