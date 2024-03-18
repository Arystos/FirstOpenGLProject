#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 crntPos;
in vec3 color;
in vec2 texCoord;
in vec3 Normal;


// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// Gets the light color and position from the main function
uniform vec4 lightColor;
uniform vec3 lightPos;

// Gets the camera position from the main function
uniform vec3 camPos;

vec4 pointLight()
{
	// Used in 2 variables
	vec3 lightVec = lightPos - crntPos;
	
	// Controls
	float distance = length(lightVec);
	float a = 3.0f;
	float b = 0.7f;
	// Light Intensity
	float intensity = 3.5 / (a * distance * distance + b * distance + 1.0f);
	
	// Ambient
	float ambient = 0.30f;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectDirection = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	float specular = specularLight * specAmount;
	
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient ) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight()
{
	// Light Intensity
	vec3 lightVec = lightPos - crntPos;
	
	// Controls
	float distance = length(lightVec);
	float a = 3.0f;
	float b = 0.7f;
	// Light Intensity
	float intensity = 150.0 / (a * distance * distance + b * distance + 1.0f);

	// Ambient
	float ambient = 0.20f;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(vec3(-0.5f , 1.0f, 1.0f));
	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectDirection = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	float specular = specularLight * specAmount;

	return (texture(diffuse0, texCoord) * (diffuse + ambient ) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// Controls
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// Ambient
	float ambient = 0.20f;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDir), 0.0f);

	// Specular
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectDirection = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	float specular = specularLight * specAmount;
	
	// Light Angle and Intensity
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient ) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}


void main()
{
	// Outputs the color of the texture
	FragColor = pointLight();
}