#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D topLeft;
uniform sampler2D topRight;
uniform sampler2D lowerLeft;
uniform sampler2D lowerRight;
uniform sampler2D shadowMap;
uniform sampler2D minAveMaxTexture;

uniform float fastExtraction;
uniform ivec2 screenSize;
uniform vec3 minAveMaxUni;
uniform vec3 sunColor;
uniform float factor; //ToDo Uniform

vec3 minAveMax;
in vec2 UV;


float lum(vec3 rgb)
{
	return dot(rgb,vec3(0.2126,0.7152,0.0722));
}

float lum3(vec3 rgb)
{
	return dot(rgb,vec3(0.33333333));
}

float key()
{
	return (minAveMax.z-minAveMax.y)/(minAveMax.z-minAveMax.x);
}

float measure()
{
	return 0.3 + 0.7*pow(key(),1.4);
}

float constant()
{
	float f=exp(factor);
	return pow((f*minAveMax.y),measure());
}

vec4 reinhard(vec4 color)
{
	return color/(color + constant());
}

void main() 
{ 
	
	if (fastExtraction == 0)
		minAveMax = minAveMaxUni;
	else
		minAveMax = texture(minAveMaxTexture,vec2(0.0,0.0)).rgb; 

	// if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
	// 	colorOut = reinhard(texture(topLeft,UV*2-vec2(0,1)));
	// else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
	// 	colorOut = reinhard(texture(topRight,UV*2-vec2(1,1)));
	// else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
	// 	colorOut = reinhard(texture(lowerRight,UV*2-vec2(1,0)));
	// else if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
	// 	colorOut = reinhard(texture(lowerLeft,UV*2));



	//if (gl_FragCoord.x < screenSize.x/4 && gl_FragCoord.y < screenSize.y/4 )
	//	colorOut = reinhard(vec4(sunColor,1));



	// if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
	// 	colorOut = texture(topLeft,UV*2-vec2(0,1));
	// else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
	// 	colorOut = texture(topRight,UV*2-vec2(1,1));
	// else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
	// 	colorOut = texture(lowerRight,UV*2-vec2(1,0));
	// else if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
	// 	colorOut = texture(lowerLeft,UV*2);

	// if (gl_FragCoord.x > screenSize.x/2-1 && gl_FragCoord.x < screenSize.x/2 )
	// 	colorOut = vec4(1,0,0,1);
	// if (gl_FragCoord.y > screenSize.y/2-1 && gl_FragCoord.y < screenSize.y/2 )
	// 	colorOut = vec4(1,0,0,1);
	// vec3 bloom = texture(lowerRight,UV).rgb;S
	// float bloom_luminance = lum3(bloom);
	//colorOut = reinhard(texture(topLeft,UV)+texture(lowerRight,UV));
	//colorOut = texture(topLeft,UV);
	// colorOut = mix(texture(topLefgdt,UV),texture(lowerRight,UV),bloom_luminance);
	// colorOut = reinhard(texture(topLeft,UV))+reinhard(texture(lowerRight,UV)); 
	//colorOut = vec4(1,0,0,1);

	//if (gl_FragCoord.x < screenSize.x/4 && gl_FragCoord.y < screenSize.y/4 )
		//colorOut = reinhard(texture(lowerLeft,UV*4));

	//if (gl_FragCoord.x > screenSize.x*0.8-1 && gl_FragCoord.x < screenSize.x*0.8+1 )

		//colorOut = vec4(1,0,0,1);
	//if (gl_FragCoord.y > screenSize.y*0.8-1 && gl_FragCoord.y < screenSize.y*0.8+1 )
//		colorOut = vec4(1,0,0,1);
	//colorOut = mix(texture(sunlight3,UV),texture(sunlight3,UV),texture(gBuffer1,UV));
	

	//colorOut = texture(topLeft,UV);
	colorOut = reinhard(texture(lowerRight,UV));

}