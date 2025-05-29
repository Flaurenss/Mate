#version 460 core

in vec3 FragPos; 
in vec2 TextCoords;
in vec3 Normal;

out vec4 FragColor;

uniform bool valid;
uniform vec4 defaultColor;
uniform sampler2D diffuse1;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 lightColor = vec3(1.0, 0.9, 0.9);
    float ambientStrength = 0.3;
    float specularStrength = 0.5;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Lighting components
    vec3 ambient = ambientStrength * lightColor;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    // Base color depending on texture
    vec3 baseColor = valid ? texture(diffuse1, TextCoords).rgb : defaultColor.rgb;

    // Final shading
    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}