#version 320 es
precision mediump float;
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixLevel;

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(-TexCoord.x, TexCoord.y)), mixLevel) * vec4(ourColor, 1.0);
}