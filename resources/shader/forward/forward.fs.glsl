//FRAGMENT SHADER
#version 330

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 wsPosition;
in vec3 vsPosition;
smooth in vec3 vsNormal;
in vec2 vsUV;

in vec2 teTexCoord;
in float teHeight;

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
uniform float translucency;
uniform sampler2D translucent_map;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec2 mouse;
uniform float ambient_amount;
uniform float diffuse_amount;
uniform float shadow_amount;

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

    vec3 ambient_term  = texture(diffuse_map, vsUV).rgb * ((light_color.x+light_color.y+light_color.z)/3);

    if(texture(diffuse_map, vsUV).a < 0.2)
        discard;
    vec3 diffuse_term  = texture(diffuse_map, vsUV).rgb * cosin * light_color;
    diffuse_term = max(diffuse_term, 0.0);

    // float distanceFromLight = texture(shadow_map, projShadowcoord.st).r;

    vec3 viewVector = position;
    float cosinSpec = max( dot( normalize(normal), normalize(viewVector)), 0.0);
    vec3 specular_term = texture(specular_map, vsUV).rgb * pow(cosinSpec, shininess);
    specular_term = max(specular_term, 0.0); 

    vec3 shaded = ambient_amount * ambient_term + diffuse_amount * (diffuse_term + specular_term);
    return max(shaded, 0.0);
    //return vec3(cosin);
}

vec3 phongLamp(vec3 position, vec3 normal)
{
    vec3 wsLamp_pos = vec3(1,12,5);
    vec3 lamp_color = vec3(10.0,10.0,3.3);
    float max_distance = 15.0;

    vec3 vsLamp_pos = vec3(view * vec4(wsLamp_pos,1.0));
    vec3 light_vector = vsLamp_pos - position;
    float dis = length(light_vector);
    if (dis > max_distance)
        return vec3(0);
    light_vector = normalize(light_vector);
    float cosin = max( dot( normalize(normal), light_vector), 0.0);

    dis = dis / max_distance;
    dis = 1 - dis;

    vec4 tex = texture(diffuse_map, vsUV);
    vec3 diffuse_term = (tex.rgb * cosin * lamp_color + tex.rgb * 0.1 ) * dis ;
    diffuse_term = max(diffuse_term, 0.0);

    // vec3 viewVector = position;
    // float cosinSpec = max( dot( normalize(normal), normalize(viewVector)), 0.0);
    // vec3 specular_term = texture(specular_map, vsUV).rgb * pow(cosinSpec, shininess);
    // specular_term = max(specular_term, 0.0); 
    //vec3 shaded = vec3(dis);
    vec3 shaded = diffuse_term;// + tex.rgb * 0.5;// + specular_term;
    return max(shaded, 0.0);
}


// @source: http://gamedev.stackexchange.com/questions/56897/glsl-light-attenuation-color-and-intensity-formula
float lightAttenuation()
{
    float result, minLight, radius, a, b;
    minLight = 0.01;
    a = 0.8;
    b = 0.6;

//    Computing distance from light to fragment
    float dist = distance( vec3( light_position ), wsPosition );

//    https://www.desmos.com/calculator/nmnaud1hrw
    radius  = sqrt(1.0 / (b * minLight));
    result  = clamp(1.0 - dist/radius, 0.0, 1.0);
    result *= result;

    return result;
}

//@ source: http://de.slideshare.net/colinbb/colin-barrebrisebois-gdc-2011-approximating-translucency-for-a-fast-cheap-and-convincing-subsurfacescattering-look-7170855
vec3 translucencyFac(vec3 normal_comp, vec3 tEye)
{

    vec3  tFac;
    float lightAtt;     // Distance of Light influences the light-strenght
    float tPower;       // Value for direct translucency ( lightsource behind )
    float tDistortion;  // Subsurface Distortion ( shift the light vector )
    vec3  tThickness;   // Thickness Texturer

    lightAtt    =   lightAttenuation();
    tPower      =   1.0;
    tDistortion =   1.0;
    tThickness  =   texture(translucent_map, vsUV).rgb;

    vec3 tLight = vec3( view * vec4(light_position, 0.0) ) + normal_comp * tDistortion;

//    Originally here is used saturate(), with glsl it will only work on NVIDIA
//    GPUs. So we rewrite this using clamp()
    float dot  = pow( clamp( dot( tEye, -tLight ), 0.0, 1.0 ), tPower )
                    * 1.0;
    vec3 translucent_term    = ( vec3(dot) + ambient_amount ) * tThickness;

    return (translucent_term * translucency) * shadow_amount;
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

    float shadowMultiplier = 1 ;
    float distanceFromLight;
    if (projShadowcoord.x >= 1 || projShadowcoord.y >= 1 || 
        projShadowcoord.x <= 0 || projShadowcoord.y <= 0)
    {
        shadowMultiplier = 1.0;
    }
    else 
    {
        float shadowSum = 0;
        float bias = 0.0005; // clamp(bias, 0, 0.01);

        distanceFromLight = textureOffset(shadow_map, projShadowcoord.st,ivec2(-1,-1)).z;
        if (distanceFromLight < projShadowcoord.z-bias)
                shadowSum +=0.25;

        distanceFromLight       = textureOffset(shadow_map, projShadowcoord.st,ivec2(-1, 1)).z;
        if (distanceFromLight < projShadowcoord.z-bias)
                shadowSum +=0.25;
        
        distanceFromLight       = textureOffset(shadow_map, projShadowcoord.st,ivec2( 1,-1)).z;
        if (distanceFromLight < projShadowcoord.z-bias)
                shadowSum +=0.25;
        
        distanceFromLight       = textureOffset(shadow_map, projShadowcoord.st,ivec2( 1, 1)).z;
        if (distanceFromLight < projShadowcoord.z-bias)
                shadowSum +=0.25;

        shadowSum *= shadow_amount*0.8;

        shadowMultiplier = 1 - shadowSum;
    }

	vec3 transFac = translucencyFac( normal, -vsPosition );
    vec4 lightpos = view * vec4( light_position , 1.0);
    vec3 shaded = phong(vsPosition, lightpos, normal, diffuse_color, specular_color, shininess) + transFac;
    shaded += phongLamp(vsPosition, vsN);

    // fragcolor.rgb  = vsN;
    // fragcolor.a = 0;
    
    fragcolor = vec4(shaded * shadowMultiplier, texture(diffuse_map,vsUV).a);
    //fragcolor = vec4(shaded, texture(diffuse_map,vsUV).a);
    fragcolor2 = vec4(0.0);

	/*
    if ( (projShadowcoord.x >= 0.99 && projShadowcoord.x <= 1.01) || (projShadowcoord.y >= 0.99 && projShadowcoord.y <= 1.01) || 
         ( projShadowcoord.x >= -0.01 && projShadowcoord.x <= 0.01) || (projShadowcoord.y >= -0.01 && projShadowcoord.y <= 0.01))
    {
         fragcolor.r = 1;
         fragcolor.b = 1;
    }
    */
}


