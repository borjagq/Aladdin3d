#version 330 core

in vec3 vertexPosition; // Position from the VS.
in vec3 vertexNormal;	// Normal from the VS.
in vec3 vertexColor;	// Color from the VS.
in vec2 vertexUV;		//UV coordinates from the VS.

uniform sampler2D baseColor;	// The main texture.
uniform sampler2D specularMap;	// The specular map for texturing.
uniform vec4 lightColor;		// Light color.
uniform vec3 lightPos;			// Light position.
uniform vec3 cameraPosition;	// Position of the camera.
uniform int lives;              // Lives available.

const float shininess = 16.0;
const float lightPower = 400.0;
const float minAmbientLight = 0.25;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

out vec4 outColor; // Outputs color in RGBA.

void main() {

    // Get the UX.
    vec2 corrected_uv = vertexUV;

    // Get the heart number.
    int heart = int(floor(corrected_uv.x) + 1);
    if (heart >= 3) heart = 3;

    // Get the real ux.
    corrected_uv.x = fract(corrected_uv.x) / 2;

    // If the lives are less than what we currently have, get the bad heart.
    if (lives < heart)
        corrected_uv.x += 0.5;
    
    outColor = texture(baseColor, corrected_uv);

}
