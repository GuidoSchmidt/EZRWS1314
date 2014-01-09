//FRAGMENT SHADER
#version 400

//*** Uniform block definitions ************************************************

//*** Input ********************************************************************
in vec3 vsPosition;
in vec3 wsPosition;
in vec3 vsNormal;
in vec3 wsNormal;
in vec2 vsUV;

//*** Output *******************************************************************
layout (location = 0) out vec4 fragcolor;

//*** Uniforms *****************************************************************
uniform sampler2D material_texture;
uniform samplerCube diffuse_cube;
uniform samplerCube reflective_cube;
uniform float sky;
uniform vec3 wsCamPosition;

//*** Main *********************************************************************
void main(void)
{
    //fragcolor = mix( vec4(0.5, 0.5, 0.5, 1.0), texture(material_texture, vsUV), 0.0 );
    //fragcolor = texture(material_texture, vsUV);
    //fragcolor.rgb = vsNormal;
    

	vec3 wsViewVec = wsPosition - wsCamPosition;

	vec3 reflected = reflect(wsViewVec,wsNormal);

	vec3 diffuse_term = texture(diffuse_cube,wsNormal).rgb;
	vec3 reflective_term = texture(reflective_cube,reflected).rgb;
	vec3 lighting_term = diffuse_term*0.8+reflective_term*0.2;

   	vec4 material_color = texture(material_texture, vsUV);

	if (material_color.a<=0.5)
    	discard;
    // if (sky==1.0)
    //	fragcolor=fragcolor/100.0;
    // fragcolor.rgb = diffuse_term * vec3(material_color);
    if (sky==1.0)
    	fragcolor.rgb = material_color.rgb;
    else	
    	fragcolor.rgb = lighting_term*material_color.rgb;
    fragcolor.a = sky;
}