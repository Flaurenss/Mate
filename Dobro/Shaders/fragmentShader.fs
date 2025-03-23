#version 330 core

in vec3 ourColor;
in vec2 TextCoord;

out vec4 FragColor;

uniform bool valid;
uniform vec4 defaultColor;
uniform sampler2D texture_diffuse1;

void main()
{
    //FragColor = mix(texture(texture1, TextCoord), texture(texture2, TextCoord), 0.8);
    //FragColor = vec4(0.502, 0.502, 0.502, 1.0);
    if(valid) {
        FragColor = texture(texture_diffuse1, TextCoord); 
    }
    else {
        FragColor = defaultColor;
    }
};