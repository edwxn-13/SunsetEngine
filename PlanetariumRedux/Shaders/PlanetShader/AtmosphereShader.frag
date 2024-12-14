#version 330 core

#define PI 3.141592
#define iSteps 16
#define jSteps 8

struct Planet
{
    vec3 center;
    float radius;
    float density;
    vec3 atmosphere_colour;
};

uniform vec3 cam_dir;
uniform Planet planet;

uniform float depthBufferFC;
uniform sampler2D depthTex;

uniform mat4 inverseProjection;
uniform mat4 inverseView;

uniform vec3 sun_pos;

in float vFragDepth;


int water_shader_for_later(Planet planet, vec3 camera_forward)
{
    vec3 camera_pos = vec3(0);
    float t = dot(planet.center - camera_pos, camera_forward);
    vec3 P = camera_pos + camera_forward * t;
    float y = length(planet.center - P);

    return int(y <= planet.radius);
} 

vec2 ray_sphere(vec3 centre, float radius, vec3 origin, vec3 direction)
{
  vec3 offset = origin - centre;
  float direction_sqr = dot(direction, direction);
  float discriminant = pow(dot(direction, offset), 2) - direction_sqr * (dot(offset, offset) - radius * radius);
  if (discriminant > 0) {
    float half_length = sqrt(discriminant) / direction_sqr;
    float middle = -dot(direction, offset) / direction_sqr;
    vec2 result = vec2(middle - half_length, 2 * half_length);
    if (result.x < 0) {
      result.y = max(0, result.y + result.x);
      result.x = 0;
    };
    return result;
  } else
    return vec2(0, 0);
}


vec3 mScreenToWorld(vec3 posS) {

    float depthValue = posS.z;
    float v_depth = pow(2.0, depthValue / (depthBufferFC * 0.5));
    float z_view = v_depth - 1.0;

    vec4 posCLIP = vec4(posS.xy * 2.0 - 1.0, 0.0, 1.0);
    vec4 posVS = inverseProjection * posCLIP;

    posVS = vec4(posVS.xyz / posVS.w, 1.0);
    posVS.xyz = normalize(posVS.xyz) * z_view;

    vec4 posWS = inverseView * posVS;

    return posWS.xyz;
  }


vec2 sphere_intersection(Planet planet, vec3 r0, vec3 rd, float sphere_radius)
{
    vec3 s0 = planet.center;
    float sr = sphere_radius;

    float a = dot(rd, rd);
    vec3 s0_r0 = s0;
    float b = 2.0 * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
	float disc = b * b - 4.0 * a* c;
    if (disc < 0.0) 
    {
        return vec2(-1.0, -1.0);
    }
    
    else
    {
		return vec2(-b - sqrt(disc), -b + sqrt(disc)) / (2.0 * a);
	}

} 

vec3 atmosphere(vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {

    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    
    vec2 p = sphere_intersection(planet, r0, r, rAtmos);
    if (p.x > p.y) return vec3(0,0,0);
    p.y = min(p.y, sphere_intersection(planet, r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = sphere_intersection(planet, iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}



void main()
{
    float phong = dot(sun_pos, planet.center);

    vec3 atmos_colour = atmosphere(cam_dir, vec3(0), sun_pos - planet.center, 0.8, planet.radius, planet.radius * 1.5, vec3(0.3,0.3,0.9), 21.0e-6,  8.0e3 ,1.2e3, 0.7);

    atmos_colour = 1.0 - exp(-1.0 * atmos_colour);

    float strength = length(atmos_colour);
    gl_FragColor = vec4(vec3(0.6) ,1.0f);
    gl_FragDepth = log2(vFragDepth) * depthBufferFC * 0.5;
}