#version 330 core


struct Material {
    sampler2D diffuse;
    sampler2D specular; 
    sampler2D bump;    
    sampler2D ambient;    

	vec3 v_diffuse;
	vec3 v_bump;
	vec3 v_specular;
	vec3 v_ambient;

	bool hasTexture;

	float opacity;
	vec3 colour;
	float refectivity;
    float shininess;

	bool checkTexture()
	{
		return hasTexture;
	}
}; 

struct PointLight
{
	vec3 position; 

	float constant;
    float linear;
    float quadratic; 

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
	vec3 position; 

	float constant;
    float linear;
    float quadratic; 

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 nor;
in vec2 tex;
in vec3 FragPos;
in float vFragDepth;

uniform Material material;
uniform PointLight tSun; 

uniform vec3 camPos;
uniform float depthBufferFC;
uniform float far_plane;
uniform samplerCube depthMap;

out vec4 fragColour;

//Function Defenitions
vec3 texLight();
float shadow_calc(vec4 lightSpace);

//Light Function Defenitions 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
float ShadowCalculation(vec3 fragPos);

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
	vec3 norm = normalize(nor);
    vec3 viewDirection = normalize(camPos - FragPos);
	vec3 result = CalcPointLight(tSun, norm, viewDirection);

	gl_FragDepth = log2(vFragDepth) * depthBufferFC * 0.5;

    fragColour = vec4(result, 1.0);
}


//Light Functions Implementation
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 ambient = light.ambient * material.v_ambient;
    vec3 diffuse = light.diffuse * diff * material.v_diffuse;
    vec3 specular = light.specular * spec * material.v_specular;

	ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculation(FragPos);                      
	return (ambient + (1 - shadow) * (diffuse + specular)) * material.v_diffuse;
}


float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - tSun.position;
	float currentDepth = length(fragToLight);
	float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

	return shadow;
}