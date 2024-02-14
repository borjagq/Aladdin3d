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

const float shininess = 16.0;
const float lightPower = 400.0;
const float minAmbientLight = 0.25;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

out vec4 outColor; // Outputs color in RGBA.

void main() {

    // Get the normal ready to use.
    vec3 normal = normalize(vertexNormal);

    // Get the light direction.
    vec3 lightDir = lightPos - vertexPosition;

    // Get the distance from the light to this fragment.
    float distance = length(lightDir);
    distance = distance * distance;
    
    // Normalize the light direction as a vector.
    lightDir = normalize(lightDir);

    // Get the lambertian component as stated in the docs.
    float lambertian = max(dot(lightDir, normal), 0.0);

    // Get the base color from the texture.
    vec4 textureColor = vec4(vertexColor, 1.0);
    
    // init the specular.
    float specular = 0.0;

    // Get the shininess that would go in this fragment.
    float fragmentShininess = shininess * 0.5;

    if (lambertian > 0.0) {

        // Get the direction from the position to the camera as a vector.
        vec3 viewDir = normalize(-vertexPosition);

        // Blinn-phong calculations.
        vec3 halfAngle = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfAngle, normal), 0.0);
        specular = pow(specAngle, fragmentShininess);
       
    }

    // Get the minimum color.
    vec3 ambientColor = vec3(textureColor) * minAmbientLight;

    // Get the diffuse final color.
    vec3 diffuseColor = vec3(textureColor) * lambertian * vec3(lightColor) * lightPower / distance;
    
    // Get the specular final color.
    vec3 specularColor = vec3(textureColor) * specular * vec3(lightColor) * lightPower / distance;

    // Get the final color that would go in the fragment.
    vec3 fragmentColor = ambientColor + diffuseColor + specularColor;
    
    // Apply gamma correction.
    fragmentColor = pow(fragmentColor, vec3(1.0 / screenGamma));
    
    outColor = vec4(fragmentColor, 1.0);

}
