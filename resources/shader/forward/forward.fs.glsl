//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsPosition;
in vec3 vsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform mat4 model;
uniform mat4 view;
uniform vec3 diffuse_color;
uniform sampler2D diffuse_map;
uniform vec3 specular_color;
uniform sampler2D specular_map;
uniform float shininess;
uniform sampler2D normal_map;
uniform vec3 light_positon;
uniform vec3 light_color;
uniform vec2 mouse;

//*** Functions ****************************************************************
// Normal mapping: calculate cotangents
// @source: http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, 1.5 * N );
}

// Normal mapping: 
// @source: http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
    vec3 map = texture(normal_map, vsUV).xyz;
    map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, V, vsUV);
    return normalize(TBN * map);
}

vec3 phong(in vec3 position, in vec4 light_positon, in vec3 normal, in vec3 diffuse_color, in vec3 specular_color, in float shininess)
{
	vec3 light_vector = normalize(light_positon.xyz - position);
	float cosin = max( dot( normalize(normal), light_vector), 0.0);

	vec3 ambient_term  = vec3(0.05);

	vec3 diffuse_term  = texture(diffuse_map, vsUV).rgb * cosin;
   	diffuse_term = clamp(diffuse_term, 0.0, 1.0);

	vec3 specular_term = texture(specular_map, vsUV).rgb * pow(cosin, shininess);
   	specular_term = clamp(specular_term, 0.0, 1.0); 

	vec3 shaded = ambient_term + diffuse_term + specular_term;
	return clamp(shaded, 0.0, 1.0);
}

//*** Main *********************************************************************
void main(void)
{

	vec3 vsN = vsNormal;
  	vec3 vsV = normalize(vsPosition);
  	vec3 vsPN = perturb_normal(vsN, vsV, vsUV);
	vec3 normal = vsPN;
	
	vec3 lightpos = light_positon;
	lightpos.y += mouse.y * 100.0;
	vec4 lightPosition = view * vec4( lightpos , 1.0);
	vec3 shaded = phong(vsPosition, lightPosition, normal, diffuse_color, specular_color, shininess);

    fragcolor = vec4(shaded, 1.0);
}