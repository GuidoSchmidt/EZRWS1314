#version 330

uniform sampler2D color;
uniform sampler2D factor;

uniform float brightness;

in vec2 varTexCos;

layout(location = 0) out vec4 color0;

float size = 256;
float step = 1/size;
vec3 sum=vec3(0);
vec2 tex;

float luminance(vec3 rgb)
{
	return 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b;
}

void main() 
{ 
	tex=vec2(-step,-step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*1;
	tex=vec2(0    ,-step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*2;
	tex=vec2( step,-step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*1;
	
	tex=vec2(-step,0);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*2;
	tex=vec2(0    ,0);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*4;
	tex=vec2( step,0);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*2;
	
	tex=vec2(-step,step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*1;
	tex=vec2(0    ,step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*2;
	tex=vec2( step,step);
	sum += vec3(texture(color,varTexCos+tex).rgb)*texture(factor,varTexCos+tex).a*1;

	sum/=16.0;
	sum.b*=0.2;
	sum.g*=0.7;
	sum*=brightness;
	color0.rgb = sum;
}