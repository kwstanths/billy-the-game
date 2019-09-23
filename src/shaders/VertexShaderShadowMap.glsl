#version 330 core

/* Input vertex attributes */
layout(location = 0) in vec3 vertex_position_modelspace;

/* Values that stay constant */
uniform mat4 matrix_lightspace;
uniform mat4 matrix_model;

void main(){

    gl_Position = matrix_lightspace * matrix_model * vec4(vertex_position_modelspace, 1);
	
}