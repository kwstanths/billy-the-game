#version 330 core

/* Input vertex attributes */
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in mat4 matrix_model;

/* Values that stay constant */
uniform mat4 matrix_lightspace;

void main(){

    gl_Position = matrix_lightspace * matrix_model * vec4(vertex_position_modelspace, 1);
	
}