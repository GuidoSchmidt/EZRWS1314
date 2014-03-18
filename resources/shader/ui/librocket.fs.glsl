#version 330

uniform sampler2D texture;

in vec2 texCoord;
in vec4 fragColor;

void main(void)
{
    vec4 objectColor = texture2D(texture, texCoord);
    gl_FragColor = vec4(objectColor.rgb * fragColor, 1.0);
}