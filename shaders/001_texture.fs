#version 320 es
precision mediump float;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform float mixLevel;

void main()
{
    FragColor = texture(texture0, TexCoord) * vec4(ourColor, 1.0);
}