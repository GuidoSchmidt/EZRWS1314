//VERTEX SHADER
#version 130

void main ( void )
{
	vec3 vsPosition = vec3 ( model * vec4 ( position, 1.0 ) );
	vec3  vsNormal = normalize ( vec3 ( model * view * vec4 ( normal, 0.0 ) ));
	vec2 vsUV = uv;
	vec4 gl_Position = projection * view * model * vec4 ( position, 1.0 );
}