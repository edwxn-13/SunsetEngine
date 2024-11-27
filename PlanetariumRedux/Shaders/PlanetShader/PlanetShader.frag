#version 330 core


struct Material {
    sampler2D diffuse;
    sampler2D specular; 
    sampler2D bump;    
    sampler2D bumpS;    

	vec3 v_diffuse;
	vec3 v_bump;
	vec3 v_specular;

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

in vec3 nor;
in vec2 tex;
in vec3 FragPosWorldSpace;
in vec4 fragPosLightSpace;
in vec3 tcol;

uniform Material material;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 camPos;
uniform vec3 lightPos;
uniform float opacity;

uniform sampler2D shadowMap;

out vec4 fragColour;

float shadow_calc(vec4 lightSpace)
{
	vec3 projCoords = lightSpace.xyz / lightSpace.w;
	projCoords = projCoords * 0.5 + 0.5; 
	float closestDepth = texture(shadowMap, projCoords.xy).r; 
	float currentDepth = projCoords.z;  
	float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;  


	return shadow;
}

vec3 CalcDirectionalIllumination()
{
	vec3 normal = normalize(nor);
	vec3 lDirection = normalize(-lightDirection);
	float diffuse = max(dot(normal,lDirection), 0.0);
	vec3 diffuseVec = diffuse * lightColour;


	vec3 nFromLight = normalize(lightDirection);
	vec3 nReflection = reflect(nFromLight, normal);


	vec3 camDir = camPos - FragPosWorldSpace;
	vec3 nCamDir = normalize(camDir);

	float spec = max(dot(nCamDir,nReflection), 0.0);

	float ambient = 0.2;

	return ( ambient + diffuseVec + spec);
}

float CalcSpotIllumination()
{
	vec3 normal = normalize(nor);
	vec3 lDirection = normalize(lightPos - FragPosWorldSpace);
	vec3 distanceeee = lightPos - FragPosWorldSpace;

	float diffuse = max(dot(normal,lDirection), 0.0);

	vec3 nFromLight = -lDirection;
	vec3 nReflection = reflect(nFromLight, normal);


	vec3 camDir = camPos - FragPosWorldSpace;
	vec3 nCamDir = normalize(camDir);

	float spec = max(dot(nCamDir,nReflection), 0.0);

	float ambient = 0.2;
	float attenuation = length(distanceeee) * 0.05;
	

	vec3 lSpotDir = normalize(lightDirection);

	float cutoff = cos(0.3);

	float theta = dot(nFromLight,lSpotDir);

	float phong;

	if(theta > cutoff)
	{
		phong = (ambient + diffuse + spec) * attenuation;
	}
	else
	{
		phong = ambient * attenuation;
	}

	return phong;
}


vec3 CalcPositionalIllumination()
{
	vec3 normal = normalize(nor);
	vec3 lDirection = normalize(lightPos - FragPosWorldSpace);
	vec3 distanceeee = lightPos - FragPosWorldSpace;

	float diffuse = max(dot(normal,lDirection), 0.0);
	vec3 diffuseVec = diffuse * lightColour;

	vec3 nFromLight = -lDirection;
	vec3 nReflection = reflect(nFromLight, normal);


	vec3 camDir = camPos - FragPosWorldSpace;
	vec3 nCamDir = normalize(camDir);

	float spec = max(dot(nCamDir,nReflection), 0.0);

	float ambient = 0.2;
	float attenuation = length(distanceeee) * 0.05;
	return ( ambient + diffuseVec + spec) * attenuation;
} 

vec3 texLight()
{

	vec3 colour = tcol;

	float amb = 0.2;
	vec3 ambient = amb * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(nor);
    vec3 lightDir = normalize(lightPos - FragPosWorldSpace);

	vec3 lDirection = normalize(lightPos - FragPosWorldSpace);
	float diff = max(dot(norm,lDirection), 0.0);
    vec3 diffuse =  diff * lightColour;  
    
    // specular
    vec3 viewDir = normalize(camPos - FragPosWorldSpace);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = 0.8 * material.v_specular;  
    float shadow = shadow_calc(fragPosLightSpace);

    vec3 result = (ambient + (1 - shadow) * (diffuse + specular)) * colour;


	return result;
}

void main()
{
	float phong = CalcSpotIllumination();
	vec3 lightTexTex = texLight() * lightColour;

	fragColour = vec4(lightTexTex, material.opacity);

}
