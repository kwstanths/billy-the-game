#version 330 core

/* Input vertex attributes */
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_color;

/* Output data to be passed to the fragment shader */
out vec3 fragment_color;

/* Values that stay constant */
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(){

    gl_Position =  matrix_projection * matrix_view * matrix_model * vec4(vertex_position_modelspace,1);

    fragment_color = vertex_color;
}
