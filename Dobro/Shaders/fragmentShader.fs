#version 330 core

in vec3 ourColor;
in vec2 TextCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = mix(texture(texture1, TextCoord), texture(texture2, TextCoord), 0.8);
    FragColor = vec4(0.502, 0.502, 0.502, 1.0);
};