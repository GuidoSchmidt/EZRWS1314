#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D blurSampler;
uniform sampler2D maskSampler;

uniform ivec2 screenSize;
uniform vec3 sunPos;

in vec2 blurUV;
in vec2 maskUV;
in vec2 sunUV;

vec3 l = vec3(0.2126, 0.7152, 0.0722); 

float lum(vec4 col)
{
	return dot(l,col.rgb);
}

void main() 
{ 
	if (sunPos.z <= 0) 
	{
		colorOut = vec4(0,0,0,1);
		return;
	}
	//vec2 ts = sunUV.xy+0.5;///sunUVPos.z;
	vec4 blur = texture(blurSampler,blurUV);
	//blured*=lum(blured);
	vec4 mask= texture(maskSampler,maskUV)*1.067-0.0669;
	colorOut =  mask * blur;

	// if (blurUV.x > sunPos.x-0.01 && blurUV.x < sunPos.x+0.01 &&
	//     blurUV.y > sunPos.y-0.01 && blurUV.y < sunPos.y+0.01)
	//     colorOut = vec4(100,100,0,1);
}