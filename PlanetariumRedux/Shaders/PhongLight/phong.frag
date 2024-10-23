#version 450 core

layout (location = 0) out vec4 fColour;

in vec3 col;
in vec3 nor;
in vec3 FragPosWorldSpace;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 camPos;
uniform vec3 lightPos;

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

void main()
{	
	vec3 phong =  CalcDirectionalIllumination();
	//float phong =  CalcSpotIllumination();

	fColour = vec4(phong * col * lightColour, 1.f);
}
