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
uniform sampler2D   specular_tex;   // set ( model wrong? )
uniform vec4        LightPosition;  // set
//uniform vec3      LightIntensity; // not used here
uniform float       Shininess;      // set



//*** Functions ****************************************************************
vec3 ads()
{
    vec3 n = normalize( vec3(1.0, 0.6, 0.0) );
    vec3 s = normalize( vec3( LightPosition ) - m_position );
    vec3 v = normalize( vec3( -m_position ) );
    vec3 r = reflect( -s, n );

//    return LightIntensity * ( Ka
//            + Kd * max( dot(s, n), 0.0 )
//            + Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );

    return 1.0 * (texture(diffuse_tex, vsUV).rgb * max( dot(s, n), 0.0 )
               + texture(specular_tex, vsUV).rgb * pow( max( dot(v,r), 0.0 ),
                 Shininess ) );
//    return vec3(1.0, 0.0, 1.0);
}

//*** Main *********************************************************************
void main(void)
{
    // vec3 vsN = normalize(vsNormal);

//    vec3 ambientColor = 1.0 * texture(diffuse_tex, vsUV).rgb;

    fragcolor = vec4( ads(), 1.0 );
//    fragcolor = vec4(ambientColor, 1.0);
}
