#version 330 core

/* Input vertex attributes */
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal_modelspace;

/* Output data to be passed to each fragment */
out vec2 fragment_uv;
out vec3 fragment_position_worldspace;
out vec3 fragment_normal_cameraspace;
out vec3 fragment_eye_direction_cameraspace;
out vec3 fragment_light_direction_cameraspace;

/* Constant values for the whole mesh. */
uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform vec3 light_position_worldspace;

void main(){

    /* Set the position of the vertex in OpenGL clip space */
	gl_Position =  matrix_projection * matrix_view * matrix_model * vec4(vertex_position_modelspace,1);
	
	/* Set the position of this vertex in worldsapce to be used from the frgament shader */
	fragment_position_worldspace = (matrix_model * vec4(vertex_position_modelspace,1)).xyz;
	
	/* 
        A vector that goes from the vertex to the camera, in camera space
        In camera space, the camera is at the origin (0,0,0).
    */
	vec3 vertex_position_cameraspace = ( matrix_view * matrix_model * vec4(vertex_position_modelspace,1)).xyz;
	fragment_eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;

	/* Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity */
	vec3 light_position_cameraspace = ( matrix_view * vec4(light_position_worldspace,1)).xyz;
    /* This is actually substraction, since the fragment_eye_direction_cameraspace is inverted above */
	fragment_light_direction_cameraspace = light_position_cameraspace + fragment_eye_direction_cameraspace;
	
	/* Normal of the the vertex, in camera space */
    /* Only correct if the matrix_model does not scale the model ! Use its inverse transpose if not */
	fragment_normal_cameraspace = ( matrix_view * matrix_model * vec4(vertex_normal_modelspace,0)).xyz; 	

	/* UV of the vertex. No special space for this one */
	fragment_uv = vertex_uv;
}

