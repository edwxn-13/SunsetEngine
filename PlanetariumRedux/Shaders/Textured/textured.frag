#version 330 core


struct Material {
    sampler2D diffuse;
    sampler2D specular; 
    sampler2D bump;    
    sampler2D bumpS;    


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

uniform Material material;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 camPos;
uniform vec3 lightPos;
uniform float opacity;

out vec4 fragColour;


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
	float amb = 0.2;
	vec3 ambient = amb * texture(material.diffuse, tex).rgb;
  	
    // diffuse 
    vec3 norm = normalize(nor);
    vec3 lightDir = normalize(lightPos - FragPosWorldSpace);

	vec3 lDirection = normalize(lightPos - FragPosWorldSpace);
	float diff = max(dot(norm,lDirection), 0.0);
    vec3 diffuse =  diff * texture(material.bumpS, tex).rgb;  
    
    // specular
    vec3 viewDir = normalize(camPos - FragPosWorldSpace);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = 0.8 * texture(material.bumpS, tex).rgb;  
        
    vec3 result = ambient + diffuse + specular;

	return result;
}

void main()
{
	float phong = CalcSpotIllumination();
	vec3 lightTexTex = texLight() * lightColour;
	//fragColour = texture(material.specular, tex).rgb * texture(material.bumpS, tex).rgb * lightColour * phong;
	//fragColour = texture(material.bumpS, tex);

	fragColour = vec4(lightTexTex, opacity);

}
