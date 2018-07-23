#version 330 core

/* Fragment shader output */
out vec3 color;

/* Vertex color */
uniform vec3 fragment_color;

void main(){

    color = fragment_color;

}