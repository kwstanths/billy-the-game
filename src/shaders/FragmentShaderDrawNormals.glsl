#version 330 core
out vec4 FragColor;

uniform vec3 fragment_color;

void main()
{
    FragColor = vec4(fragment_color, 1.0);
}