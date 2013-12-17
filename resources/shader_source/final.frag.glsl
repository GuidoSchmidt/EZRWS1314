#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D topLeft;
uniform sampler2D topRight;
uniform sampler2D lowerLeft;
uniform sampler2D lowerRight;

uniform ivec2 screenSize;

in vec2 UV;

void main() 
{ 
	if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
		colorOut = texture(topLeft,UV*2-vec2(0,1));
	else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
		colorOut = texture(topRight,UV*2-vec2(1,1));
	else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
		colorOut = texture(lowerRight,UV*2-vec2(1,0));
	else if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
		colorOut = texture(lowerLeft,UV*2);

	if (gl_FragCoord.x > screenSize.x/2-1 && gl_FragCoord.x < screenSize.x/2+1 )
		colorOut = vec4(1,0,0,1);
	if (gl_FragCoord.y > screenSize.y/2-1 && gl_FragCoord.y < screenSize.y/2+1 )
		colorOut = vec4(1,0,0,1);

	colorOut = texture(lowerRight,UV)+texture(topLeft,UV);

	//if (gl_FragCoord.x > screenSize.x*0.8-1 && gl_FragCoord.x < screenSize.x*0.8+1 )

		//colorOut = vec4(1,0,0,1);
	//if (gl_FragCoord.y > screenSize.y*0.8-1 && gl_FragCoord.y < screenSize.y*0.8+1 )
//		colorOut = vec4(1,0,0,1);
	//colorOut = mix(texture(sunlight3,UV),texture(sunlight3,UV),texture(gBuffer1,UV));
}