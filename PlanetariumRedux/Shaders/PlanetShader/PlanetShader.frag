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

in vec3 nor;
in vec2 tex;
in vec3 FragPos;
in vec3 tcol;
in float vFragDepth;

uniform Material material;
uniform PointLight tSun; 

uniform vec3 camPos;
uniform float depthBufferFC;
uniform samplerCube depthMap;
uniform float far_plane;

out vec4 fragColour;

//Defenitions
vec3 biome_colour(float temperature, float percipitation, float height);

//Light Defenitions
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 col);
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
	vec3 colour = biome_colour(tcol.x,tcol.y,tcol.z);

	vec3 norm = normalize(nor);
    vec3 viewDirection = normalize(camPos - FragPos);
	vec3 result = CalcPointLight(tSun, norm, viewDirection, colour);
	fragColour = vec4(result, 1.0);
	gl_FragDepth = log2(vFragDepth) * depthBufferFC * 0.5;
}


vec3 biome_colour(float temperature, float percipitation, float height)
{
	vec3 SNOW = vec3(0.9255, 0.9647, 1.0);
	vec3 BOREAL = vec3(0.2863, 0.8686, 0.098);
	vec3 TEMPRATE = vec3(0.3294, 0.6392, 0.0392);
	vec3 TROPICAL = vec3(0.11, 0.54,0.15);

	vec3 TUNDRA = vec3(0.68, 0.76, 0.54);
	vec3 SHRUB = vec3(0.75,0.59,0.15);
	vec3 DESERT = vec3(0.86, 0.79, 0.27);

	vec3 MOUNTAIN = vec3(0.5255, 0.5255, 0.5255);

	vec3 wetBiome = SNOW;

	wetBiome = mix(wetBiome, BOREAL, ((temperature)));
	wetBiome = mix(wetBiome, TEMPRATE, ((temperature)));
	wetBiome = mix(wetBiome, TROPICAL, ((temperature)));

	vec3 dryBiome = SNOW;
	dryBiome = mix(wetBiome, TUNDRA, ((temperature)));
	dryBiome = mix(wetBiome, SHRUB, ((temperature)));
	dryBiome = mix(wetBiome, DESERT, ((temperature)));


	vec3 oceon_col = mix(vec3(0.11,0.23,0.8), vec3(0.22,0.46,1.0), 1-height);
	vec3 land_col = mix(dryBiome, wetBiome, percipitation);

	//and_col = mix(land_col,MOUNTAIN, height/2);

	land_col = mix(oceon_col,land_col,smoothstep(0.025,0.04,height));
	
	return land_col;
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

//vec3 result = (ambient + (1 - shadow) * (diffuse + specular)) * colour;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 col)
{
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

	float dark_side_attenuation = dot(normal, lightDir);

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
	return (ambient + (1 - shadow) * (diffuse + specular)) * col;
}