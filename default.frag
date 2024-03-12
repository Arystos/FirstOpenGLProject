#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;

uniform vec4 lightColor;

uniform vec3 lightPos;

uniform vec3 camPos;


void main()
{
	// Ambient
	float ambient = 0.3;
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDir), 0.0);
	
	// Specular
	float specularLight = 0.5;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectDirection = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 8);
	float specular = specularLight * specAmount;
	
	// Outputs the color of the texture
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}