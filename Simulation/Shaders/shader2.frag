#version 410 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 Position;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D planeMap;

void main()
{
    float ambConst = 0.05;
    float diffConst = 0.8;
    float specConst = 0.07;
    float shininess = 8.0;
    vec3 color = texture(planeMap, TexCoords).rgb;
    
    // ambient
    vec3 ambient = ambConst * color;
    
    // diffuse
    vec3 lightRay = normalize(lightPos - Position);
    vec3 normal = normalize(Normal);
    vec3 diffuse = diffConst * max(dot(lightRay, normal), 0.0) * color;
    
    // specular (for blinn-phong)
    vec3 view = normalize(viewPos - Position);
    vec3 halfway = normalize(lightRay + view);
    vec3 specular = vec3(specConst) * pow(max(dot(normal, halfway), 0.0), shininess);
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor= texture(planeMap, TexCoords);
}


