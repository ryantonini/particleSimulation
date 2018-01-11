#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 Position;

uniform mat4 projection;
uniform mat4 view;

void main()
{

    TexCoords = aTexCoords;
    Normal = aNormal;
    Position = aPos;
    vec4 temp = vec4(aPos, 1.0);
   //temp.x = temp.x + 25;
   //temp.y = temp.y + 30;
    gl_Position = projection * view * temp;
}


