#version 330 core

in vec4 voColor;

out vec4 FragColor;

uniform vec4 color;

void main()
{             
    FragColor = color;
}