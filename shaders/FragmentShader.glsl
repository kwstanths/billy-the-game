#version 330 core

/* Interpolated values from the vertex shaders */
in vec2 fragment_uv;
in vec3 fragment_position_worldspace;
in vec3 fragment_normal_cameraspace;
in vec3 fragment_eye_direction_cameraspace;
in vec3 fragment_light_direction_cameraspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D Texture;
uniform vec3 light_position_worldspace;

void main(){

	/* Light emission properties. Should probably be uniforms */
	vec3 light_color = vec3(1,1,1);
    float light_power = 350.0f;
	
	/* Material properties */
    /* Diffure color: Color beacause of direct light */
	vec3 material_diffuse_color = texture( Texture, fragment_uv ).rgb;
    /* 
        Ambient color: Color because of indirect light, in the back of a model let's say 
        Not actually computing it, just cheating with some a flat 10% light in every surface 
    */
	vec3 material_ambient_color = vec3(0.1,0.1,0.1) * material_diffuse_color;
    /*
        The circle with high density light that is created when the light source is
        directed at a surface
    */
	vec3 material_specular_color = vec3(0.3,0.3,0.3);

	/* Distance to the light source */
	float distance = length( light_position_worldspace - fragment_position_worldspace );

	/* Normalizing the surface normal of the computed fragment, in camera space */
	vec3 n = normalize( fragment_normal_cameraspace );
	/* Normalizing the direction of the light (from the fragment to the light) */
	vec3 l = normalize( fragment_light_direction_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(fragment_eye_direction_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color =
        // Ambient : simulates indirect lighting, for example behind an object
        material_ambient_color + 
		// Diffuse : "color" of the object
	    material_diffuse_color * light_color * light_power * cosTheta / (distance*distance) +
        // Specular: reflective highlightm like a mirror, for example that bright spot that indicates where the light source is
        material_specular_color * light_color * light_power * pow(cosAlpha,5) / (distance*distance);
    
}
