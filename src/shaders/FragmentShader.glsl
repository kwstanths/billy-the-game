#version 330 core

/* Interpolated values from the vertex shader */
in vec3 fragment_color;

/* Fragment shader output */
out vec3 color;

void main(){

    color = fragment_color;

}
