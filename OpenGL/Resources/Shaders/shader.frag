#version 330 core

//---------------------------------

struct Light
{
    vec3  position;
    vec4  color;
    float ambient;
    float diffuse;
    float specular;
};

#define LIGHTS_MAX 32

//---------------------------------

in vec3 FragPos;
in vec4 FragColor;
in vec2 FragTexCoord;
in vec3 FragNormal;

uniform vec3 viewPos;

uniform sampler2D shapeTexture;
uniform bool      useTexture;
uniform bool      useLightning;
uniform bool      useBloom;
uniform float     time;

uniform Light lights[LIGHTS_MAX];

out vec4 color;

//---------------------------------

float rand(float co);
vec3 CalculateLightning(Light light, vec3 normal, vec3 viewpos, vec3 fragpos);

//---------------------------------

void main()
{
    vec4 fragColor = useTexture? texture(shapeTexture, FragTexCoord): FragColor;

    if (useLightning)
    {
        vec3 result = vec3(0, 0, 0);
        vec3 normal = normalize(FragNormal);
        for (int i = 0; i < LIGHTS_MAX; i++)
            result += CalculateLightning(lights[i], normal, viewPos, FragPos);

        color = fragColor*vec4(result, 1);
    }
    else color = fragColor;

    if (useBloom) 
        color = vec4(color.xyz*rand(time*FragPos.x*FragPos.y*0.001), 1);
}

//---------------------------------

float rand(float co)
{ 
    return fract(sin(co*(91.3458)) * 47453.5453)*1.2;
}

//---------------------------------

vec3 CalculateLightning(Light light, vec3 normal, vec3 viewpos, vec3 fragpos)
{
    // Ambient lightning
    vec3 ambient = light.color.xyz*light.ambient;

    // Diffuse lightning
    vec3 lightDir = normalize(light.position-fragpos);
    float diff = max(dot(normal, lightDir), .0f);
    vec3 diffuse = light.color.xyz*light.diffuse*diff;

    // Specular lightning
    vec3 viewDir = normalize(viewpos-light.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), .0f), 64);
    vec3 specular = light.color.xyz*light.specular*spec;

    return ambient+diffuse+specular;
}

//---------------------------------