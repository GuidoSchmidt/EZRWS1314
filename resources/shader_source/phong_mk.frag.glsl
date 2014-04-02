//FRAGMENT SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 m_normal; // set
in vec3 m_position; // set
in vec2 vsUV; // set

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D diffuse_tex; // set
uniform vec4 LightPosition;
uniform vec3 LightIntensity;
uniform vec3 Kd;         // Diffuse reflectivity
uniform vec3 Ka;         // Ambient reflectivity
uniform vec3 Ks;         // Specular reflectivity
uniform float Shininess; // Specular shininess factor

//*** Functions ****************************************************************
vec3 ads()
{
    vec3 n = normalize( m_normal );
    vec3 s = normalize( vec3( LightPosition )- m_position );
    vec3 v = normalize( vec3( -m_position ) );
    vec3 r = reflect( -s, n );

    return LightIntensity * ( Ka + Kd * max( dot(s, n), 0.0 ) +
            Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

//*** Main *********************************************************************
void main(void)
{
    // vec3 vsN = normalize(vsNormal);

    vec3 ambientColor = 1.0 * texture(diffuse_tex, vsUV).rgb;

    //fragcolor = vec4( ads(), 1.0 );
    fragcolor = vec4(ambientColor, 1.0);
}
