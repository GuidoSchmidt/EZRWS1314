//FRAGMENT SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 m_normal;   // set
in vec3 m_position; // set
in vec2 vsUV;       // set

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D   diffuse_tex;    // set
uniform sampler2D   specular_tex;   // set
uniform sampler2D   normal_tex;     // set
uniform vec4        LightPosition;  // set
uniform float       Shininess;      // set



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
    vec3 map = texture(normal_tex, vsUV).xyz;
    map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, V, vsUV);
    return normalize(TBN * map);
}


vec3 ads(vec3 normal_comp)
{
    vec3 n = normalize( normal_comp );
    vec3 s = normalize( vec3( LightPosition ) - m_position );
    vec3 v = normalize( vec3( -m_position ) );
    vec3 r = reflect( -s, n );

    return 1.0 * (texture(diffuse_tex, vsUV).rgb * max( dot(s, n), 0.0 )
               + texture(specular_tex, vsUV).rgb * pow( max( dot(v,r), 0.0 ),
                 Shininess ) );
}

//*** Main *********************************************************************
void main(void)
{
//    calculating the normal
    vec3 vsN = normalize(m_normal);
    vec3 vsV = normalize(m_position);
    vec3 vsPN = perturb_normal(vsN, vsV, vsUV);
    vec3 normal = vsPN;

    fragcolor = vec4( ads(normal), 1.0 );
}
