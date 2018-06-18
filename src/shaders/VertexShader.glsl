#version 330 core

/* Input vertex attributes */
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

/* Output data to be passed to the fragment shader */
out vec2 uv;
out vec3 normal;
out vec3 fragment_position_globalspace;

/* Values that stay constant */
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main(){

	/* Use the following in case of non uniform scaling, to calculate the normal */
	/* normal = mat3(transpose(inverse(matrix_model))) * vertex_normal; */

    gl_Position =  matrix_projection * matrix_view * matrix_model * vec4(vertex_position_modelspace,1);

	/* Set output */
	/* Calculate the position of the fragment in the global space */
	fragment_position_globalspace = vec3(matrix_model * vec4(vertex_position_modelspace, 1.0));
	normal = vertex_normal;
    uv = vertex_uv;
}
