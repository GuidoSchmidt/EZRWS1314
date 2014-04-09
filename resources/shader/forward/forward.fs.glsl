//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 wsPosition;
in vec3 vsPosition;
smooth in vec3 vsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;
layout (location = 1) out vec4 fragcolor2;

//*** Uniforms *****************************************************************
//uniform samplerCube diffuse_cube;
//uniform samplerCube reflective_cube;
//uniform float sky;
uniform mat4 model;
uniform mat4 view;
uniform vec3 diffuse_color;
uniform sampler2D diffuse_map;
uniform vec3 specular_color;
uniform sampler2D specular_map;
uniform float shininess;
uniform sampler2D normal_map;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec2 mouse;
uniform float ambient_amount;
uniform float diffuse_amount;

uniform sampler2D shadow_map;
// uniform mat4 light_model;
uniform mat4 light_view;
uniform mat4 light_projection;
mat4 bias_matrix = mat4(0.5, 0.0, 0.0, 0.0,
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
    return mat3( T * invmax, B * invmax, 0.5 * N );
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

// Phong shading
vec3 phong(in vec3 position, in vec4 light, in vec3 normal, in vec3 diffuse_color, in vec3 specular_color, in float shininess)
{
    vec3 light_vector = normalize(position - light.xyz);
    float cosin = max( dot( normalize(normal), light_vector), 0.0);

    vec3 ambient_term  = texture(diffuse_map, vsUV).rgb * light_color;

    if(texture(diffuse_map, vsUV).a < 0.2)
        discard;
    vec3 diffuse_term  = texture(diffuse_map, vsUV).rgb * cosin * light_color;
    diffuse_term = max(diffuse_term, 0.0);

    // float distanceFromLight = texture(shadow_map, projShadowcoord.st).r;

    vec3 viewVector = position;
    float cosinSpec = max( dot( normalize(normal), normalize(viewVector)), 0.0);
    vec3 specular_term = texture(specular_map, vsUV).rgb * pow(cosinSpec, shininess);
    specular_term = max(specular_term, 0.0); 

    vec3 shaded = ambient_amount * ambient_term + diffuse_amount * diffuse_term + specular_term;
    return max(shaded, 0.0);
}

//*** Main *********************************************************************
void main(void)
{
    vec3 vsN = vsNormal;
    vec3 vsV = normalize(vsPosition);
    vec3 vsPN = perturb_normal(vsN, vsV, vsUV);
    vec3 normal = vsPN;
    
    //! shadow mapping
    vec4 shadowcoord = bias_matrix * light_projection * light_view * model * vec4(wsPosition, 1.0);
    vec4 projShadowcoord = shadowcoord / shadowcoord.w;

    float distanceFromLight = texture(shadow_map, projShadowcoord.st).z;

    float shadow = 1.0;
    vec3 light_vector = normalize(vsPosition - light_position);
    //loat cosTheta = clamp(dot( vsN,light_vector ),0.0,1.0);
    //float bias = 0.005*tan(acos(cosTheta));
    float bias = 0.00; // clamp(bias, 0, 0.01);
    if (distanceFromLight < projShadowcoord.z-bias)
    {
        shadow = 0.3;
    }

    vec4 lightpos = vec4( light_position , 1.0);
    //lightpos.y += mouse.y * 100.0;
    lightpos = view * lightpos;
    //vec3 shaded = texture(diffuse_map, vsUV).rgb; // <-- Use for testing
    vec3 shaded = phong(vsPosition, lightpos, normal, diffuse_color, specular_color, shininess);

    // fragcolor.rgb  = vsN;
    // fragcolor.a = 0;
    fragcolor = vec4(shaded * shadow, texture(diffuse_map,vsUV).a);
    fragcolor2 = vec4(0.0);
}
