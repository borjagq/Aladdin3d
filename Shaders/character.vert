#version 330 core

layout (location = 0) in vec3 inVertex;	// Vertex.
layout (location = 1) in vec3 inNormal;	// Normal (not necessarily normalized).
layout (location = 2) in vec3 inColor;	// Color (not necessarily normalized).
layout (location = 3) in vec2 inUV;		// UV coordinates.

uniform mat4 Model;			// Imports the model matrix.
uniform mat4 View;			// Imports the View matrix.
uniform mat4 Projection;	// Imports the projection matrix.
uniform mat4 modelView;		// Imports the modelView already multiplied.
uniform mat4 normalMatrix;	// Imports the normal matrix.
uniform vec3 lightPos;		// Light position.
uniform float time;			// Time in seconds.
uniform float jump_start;	// Imports the modelView already multiplied.
uniform float jump_velocity;	// Imports the normal matrix.
uniform float gravity;			// Light position.
uniform float last_hit;		// The moment when the last hit ocurred.

out vec3 vertexNormal;		// Passes the normal to the fragment shader.
out vec3 vertexColor;		// Passes the color to the fragment shader.
out vec2 vertexUV;			// Passes the UV coordinates to the fragment shader.
out vec3 vertexPosition;	// Passes the current vertex to the fragment shader.

vec3 interpolate(vec3 a, vec3 b, float step) {

    // Interpolate each.
    float x = a.x + (b.x - a.x) * step;
    float y = a.y + (b.y - a.y) * step;
    float z = a.z + (b.z - a.z) * step;

    return vec3(x, y, z);

}

void main() {

	// Get step based on the last hit.
	float step = time - last_hit;
	if (step > 1.0 || step < 0.0) {
		step = 1.0;
	}

	// Get the color by interpolation.
	vertexColor = interpolate(vec3(0.9, 0.1, 0.1), vec3(0.1, 0.1, 0.1), step);

	// Assigns the direct passes.
	vertexNormal = vec3(normalMatrix * vec4(inNormal, 0.0));
	vertexUV = mat2(0.0, -1.0, 1.0, 0.0) * inUV;

	// Check if the character is jumping or not.
	if (jump_start != -1) {
	
		// Get the point after applying Model transformations.
		vec4 transformedPosition = Model * vec4(inVertex, 1.0);

		// Calculate the jump displacement.
		float jump_time = time - jump_start;
		float jump_y = jump_velocity * jump_time + 0.5 * gravity * jump_time * jump_time;

		// Make it move.
		transformedPosition.y += jump_y;

		// Calculates the vertex by applying the transforms to it.
		vec4 tmpVertexPosition = View * transformedPosition;
		vertexPosition = vec3(tmpVertexPosition) / tmpVertexPosition.w;

		// Sets the visualized position by applying the camera matrix.
		gl_Position = Projection * View * transformedPosition;
	
	} else {
	
		// Calculates the vertex by applying the transforms to it.
		vec4 tmpVertexPosition = modelView * vec4(inVertex, 1.0);
		vertexPosition = vec3(tmpVertexPosition) / tmpVertexPosition.w;

		// Sets the visualized position by applying the camera matrix.
		gl_Position = Projection * modelView * vec4(inVertex, 1.0);
	
	}

}
