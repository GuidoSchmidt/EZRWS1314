#version 440 core

layout(triangles, equal_spacing, ccw) in;

in vec3 tcPosition[];
in vec2 tcTexCoord[];

out vec2 teTexCoord;
out float teHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightMap;
uniform bool tesselation;

void main()
{
	if(tesselation)
	{

	}
	else
	{
		vec3 p0 = gl_TessCoord.x * tcPosition[0];
		vec3 p1 = gl_TessCoord.y * tcPosition[1];
		vec3 p2 = gl_TessCoord.z * tcPosition[2];
		vec3 tePosition = p0 + p1 + p2;
	 
		vec2 tc0 = gl_TessCoord.x * tcTexCoord[0];
		vec2 tc1 = gl_TessCoord.y * tcTexCoord[1];
		vec2 tc2 = gl_TessCoord.z * tcTexCoord[2];	
		teTexCoord = tc0 + tc1 + tc2;
	 
		vec3 normal = normalize(cross(tcPosition[1] - tcPosition[0], 
									  tcPosition[2] - tcPosition[0]));
	 
		float height = texture(heightMap, teTexCoord).x;
		teHeight = height;
		tePosition += normal * height;
	 
		gl_Position = projection * view * model * vec4(tePosition, 1.0);
	}
}
 