#version 410 core

in vec4 Color;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uvMap;

void main()
{
    FragColor = (texture(uvMap, TexCoords) * Color);
}

