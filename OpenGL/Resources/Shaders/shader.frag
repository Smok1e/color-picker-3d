#version 450 core

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
in mat3 FragTBN;

uniform vec3 viewPos;

uniform sampler2D shapeTexture;
uniform sampler2D shapeNormalMap;
uniform bool      useTexture;
uniform bool      useNormalMap;
uniform bool      useLightning;

uniform Light lights[LIGHTS_MAX];

out vec4 color;

//---------------------------------

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos);

//---------------------------------

void main()
{
    vec4 fragColor = useTexture? texture(shapeTexture, FragTexCoord): FragColor;
    vec3 fragNormal = useNormalMap? FragTBN*(2.0*texture(shapeNormalMap, FragTexCoord).xyz - 1.0): FragNormal;

    if (useLightning)
    {
        vec3 result = vec3(0, 0, 0);
        vec3 normal = normalize(fragNormal);
        for (int i = 0; i < LIGHTS_MAX; i++)
            result += CalculatePointLight(lights[i], normal, viewPos, FragPos);

        color = fragColor*vec4(result, 1);
    }

    else color = fragColor;
}

//---------------------------------

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos)
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
    float spec = pow(max(dot(viewDir, reflectDir), .0f), 32);
    vec3 specular = light.color.xyz*light.specular*spec;

    return ambient+diffuse+specular;
}

//---------------------------------