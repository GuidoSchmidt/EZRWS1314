//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 wsPosition;
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

uniform sampler2D shadow_map;
uniform mat4 light_model;
uniform mat4 light_view;
uniform mat4 light_projection;
mat4 bias = mat4(0.5, 0.0, 0.0, 0.0,
                 0.0, 0.5, 0.0, 0.0,
                 0.0, 0.0, 0.5, 0.0,
                 0.5, 0.5, 0.5, 1.0);

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

vec3 ads(in vec3 Position, in vec3 Normal, in vec3 LightPosition, in float Shininess)
{
    vec3 n = normalize( Normal );
    vec3 s = normalize( vec3(LightPosition) - Position );
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect( -s, n );
    return 1.0 * (texture(diffuse_map, vsUV).rgb * max( dot(s, n), 0.0 ) + texture(specular_map, vsUV).rgb * pow( max( dot(v,r), 0.0 ), Shininess ) );
}

//*** Main *********************************************************************
void main(void)
{
	vec3 vsN = normalize(vsNormal);
  	vec3 vsV = normalize(vsPosition);
  	vec3 vsPN = perturb_normal(vsN, vsV, vsUV);
	vec3 normal = vsPN;

    //! shadow mapping
    vec4 shadowcoord = bias * light_projection * light_view * model * vec4(wsPosition, 1.0); 
    vec4 projShadowcoord = shadowcoord / shadowcoord.w;

    float distanceFromLight = texture(shadow_map, projShadowcoord.st).z;

	vec4 lightPosition = vec4( vec3(0.0, 2.0, 5.0) , 1.0);

	float shadow = 1.0;
    if (distanceFromLight < projShadowcoord.z)
    {
        shadow = 0.2;
    }

    vec3 shaded = ads(vsV, normal, lightPosition.xyz, shininess);
    vec3 ambient = 0.2 * texture(diffuse_map, vsUV).rgb;

    fragcolor = vec4(ambient + shadow * vec3(shaded), 1.0);
}
