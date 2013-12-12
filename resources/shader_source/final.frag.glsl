#version 330

layout(location = 0) out vec4 colorOut;
 
uniform sampler2D gBuffer1;
uniform sampler2D sunlight1;
uniform sampler2D sunlight2;
uniform sampler2D sunlight3;

uniform ivec2 screenSize;

in vec2 UV;

void main() 
{ 
	if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
		colorOut = texture(gBuffer1,UV);
	else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y > screenSize.y/2 )
		colorOut = texture(sunlight1,UV);
	else if (gl_FragCoord.x > screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
		colorOut = texture(sunlight2,UV);
	else if (gl_FragCoord.x < screenSize.x/2 && gl_FragCoord.y < screenSize.y/2 )
		colorOut = texture(sunlight3,UV);

	if (gl_FragCoord.x > screenSize.x/2-1 && gl_FragCoord.x < screenSize.x/2+1 )
		colorOut = vec4(1,0,0,1);
	if (gl_FragCoord.y > screenSize.y/2-1 && gl_FragCoord.y < screenSize.y/2+1 )
		colorOut = vec4(1,0,0,1);

	//colorOut = texture(sunlight3,UV);
}