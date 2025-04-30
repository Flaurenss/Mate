#version 330 core

in vec3 ourColor;
in vec2 TextCoords;

out vec4 FragColor;

uniform bool valid;
uniform vec4 defaultColor;
uniform sampler2D diffuse1;

void main()
{
    if(valid) {
        FragColor = texture(diffuse1, TextCoords); 
    }
    else {
        FragColor = defaultColor;
    }
};