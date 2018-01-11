#version 410 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoords;

out vec4 Color;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 20.0f;
    Color = color;
    TexCoords = texCoords;
    vec4 temp = vec4((vertex * scale) + offset + vec3(65, 50, 45), 1.0);
    gl_Position = projection * view * temp;
}

