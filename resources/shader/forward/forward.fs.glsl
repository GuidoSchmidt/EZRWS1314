//FRAGMENT SHADER
#version 400

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
uniform vec3 specular_color;
uniform vec3 shininess;

//*** Functions ****************************************************************
vec3 phong(in vec3 position, in vec4 light_positon, in vec3 normal, in vec3 diffuse_color, in vec3 specular_color, in float shininess)
{
	vec3 light_vector = normalize(light_positon.xyz - position);
	float cosin = max( dot( normalize(normal), light_vector), 0.0);

	vec3 ambient_term  = vec3(0.05);

	vec3 diffuse_term  = diffuse_color * cosin;
   	diffuse_term = clamp(diffuse_term, 0.0, 1.0);

	vec3 specular_term = specular_color * pow(cosin, shininess);
   	specular_term = clamp(specular_term, 0.0, 1.0); 

	vec3 shaded = ambient_term + diffuse_term + specular_term;
	return clamp(shaded, 0.0, 1.0);
}

//*** Main *********************************************************************
void main(void)
{
	vec4 lightPosition = view * vec4( 220.0, 100.0, 0.0 , 1.0);
	vec3 shaded = phong(vsPosition, lightPosition, vsNormal, diffuse_color, specular_color, shininess);
    fragcolor = vec4(shaded, 1.0);
}