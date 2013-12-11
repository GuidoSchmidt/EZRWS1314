#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D blur;
uniform sampler2D mask;

uniform ivec2 screenSize;


in vec2 blurUV;
in vec2 maskUV;

vec3 l = vec3(0.2126, 0.7152, 0.0722); 

float lum(vec4 col)
{
	return dot(l,col.rgb);
}

void main() 
{ 
	vec4 blured = texture(blur,blurUV);
	//blured*=lum(blured);
	vec4 masked = texture(mask,blurUV);
	colorOut = blured * masked;
}