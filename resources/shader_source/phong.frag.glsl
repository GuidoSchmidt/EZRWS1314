#version 330
//#version 120

uniform sampler2D diffuse;
uniform sampler2D normals;

uniform ivec2 screenSize;
uniform vec2 nearFar;
uniform ivec2 mousePos;

uniform float ambientAmount;
uniform float diffuseAmount;
uniform float specularAmount;
uniform float specularFactor;
uniform float lightPower;
uniform float exposure;

in vec2 varTexCos;

layout(location = 0) out vec4 color0;

vec3 position;
//vec3 light=vec3(0.0,50,0.5);
vec3 light=vec3(0,0,0);
vec2 mouse;
float Lum; //Luminance

float linearize(float d)
{
    return (2 * nearFar.x) / (nearFar.y + nearFar.x - d * (nearFar.y-nearFar.x));
}

void moveLight()
{
	light.x=0;//mouse.x*2-1;
	light.y=2;//mouse.y*2-1+2;
	light.z=1-linearize(texture(diffuse,vec2(0.5,0.5)).a);
}

void main() 
{
	vec4 tex = texture(diffuse,varTexCos);
	vec4 nor = texture(normals,varTexCos);
	
	//calculate SS Position
	position.x=((gl_FragCoord.x/screenSize.x)-0.5)*2.0;
	position.y=((gl_FragCoord.y/screenSize.y)-0.5)*2.0;
//	position.z=tex.a;
	position.z=linearize(tex.a);

	//mousepos
	mouse.x=float(mousePos.x)/float(screenSize.x);
	mouse.y=1-float(mousePos.y)/float(screenSize.y);
	moveLight();
	
	//vectors
	vec3 L = normalize(position-light);               //light 
	vec3 N = normalize(nor.rgb);                      //normal
	vec3 E = normalize(position);                     //eye
	vec3 R = normalize(reflect(L, N));                //reflect
	
	//colors
	vec3 tDiff = tex.rgb;
	vec3 tSpec = tDiff;
	
	//components
	vec3 AM = tDiff*lightPower*0.1;
	vec3 DI = max(vec3(0.0),dot(-L,N)*tDiff*lightPower);
	vec3 SP = min(abs(tSpec*lightPower*pow(max(dot(R,E),0),specularFactor)),1000);
	vec3 rgb = ambientAmount*AM+diffuseAmount*DI+specularAmount*SP;
	rgb*=exposure;

	color0.rgb = rgb;
	color0.a = position.z;
//	if (position.x==light.x && position.y == light.y)
//	color0.rgb = vec3(1,0,0);
//	color0.rgb = toneMap(hdr);
//	color0.rgb = vec3(position.z);
	
	
//	color0.rg = gl_TexCoord[0].st;

}