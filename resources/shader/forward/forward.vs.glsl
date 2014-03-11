//VERTEX SHADER
#version 130

//*** Main *********************************************************************
void main(void)
{
	vsPosition = vec3( model * vec4(position, 1.0) );
	vsNormal = normalize(vec3(model * view * vec4(normal, 0.0) ));
	vsUV = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
}