#version 400

//--- Input --------------------------------------------------------------------
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in vec3 tePosition[3];
in vec3 tePatchDistance[3];

//--- Output -------------------------------------------------------------------
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;

//--- Uniforms -----------------------------------------------------------------
uniform mat4 model;
uniform mat3 view;

void main()
{
    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = transpose(inverse(view * model)) * vec4(normalize(cross(A, B)),0.0);
    
    gPatchDistance = tePatchDistance[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}