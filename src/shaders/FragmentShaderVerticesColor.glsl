#version 330 core

out vec4 FragColor;

/* Vertex color */
uniform vec3 fragment_color;
uniform float alpha;

void main(){

    FragColor = vec4(fragment_color, alpha);
}