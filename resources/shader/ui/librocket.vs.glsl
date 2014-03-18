#version 330

uniform vec2 translation;
uniform vec2 viewDimensions;

attribute vec2 vertexPosition;
attribute vec4 vertexColor;
attribute vec2 vertexTexCoord;

out vec2 texCoord;
out vec4 fragColor;

void main(void)
{
    vec2 ndcPos = ((vertexPosition + translation)/(viewDimensions));

    texCoord = vertexTexCoord;
    fragColor = vertexColor;
    gl_Position = vec4(ndcPos, 0.0, 1.0);
}