//FRAGMENT SHADER
#version 400

//--- Input --------------------------------------------------------------------
in vec3 normal;

//--- Output -------------------------------------------------------------------
layout (location = 0) out vec4 fragcolor;

//--- Uniforms -----------------------------------------------------------------
uniform vec3 light_position;

void main()
{
    vec3 N = normalize(gFacetNormal);
    vec3 L = light_position;
    float df = abs(dot(N, L));
    vec3 color = df * vec3(1.0, 0.0, 0.0);

    fragcolor = vec4(color, 1.0);
}