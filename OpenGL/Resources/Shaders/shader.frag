// Fragment shader
#version 450 core

//--------------------------------- Point light

struct Light
{
    vec3  position;
    vec4  color;
    float ambient;
    float diffuse;
    float specular;
};

#define LIGHTS_MAX 32

//--------------------------------- Output

layout(location=0) out vec4 out_Color;

//--------------------------------- Uniforms

uniform vec4      modelColor;
uniform sampler2D modelTexture;
uniform sampler2D modelNormalMap;
uniform sampler2D modelSpecularMap;
uniform bool      modelUseTexture;
uniform bool      modelUseNormalMap;
uniform bool      modelUseSpecularMap;
uniform bool      modelUseLightning;

uniform vec3      viewPosition;

uniform Light     lights[LIGHTS_MAX];
uniform int       lightCount;

//--------------------------------- Input

in vec3 fragment_Position;
in vec3 fragment_Normal;
in mat3 fragment_TBN;
in vec2 fragment_TexCoord;

//--------------------------------- 

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos, float intensity);

//--------------------------------- 

void main()
{
    vec4 source_color = modelUseTexture? texture(modelTexture, fragment_TexCoord): modelColor;
    if (modelUseLightning)
    {
        vec3 normal = modelUseNormalMap? fragment_TBN * normalize(2.f * texture(modelNormalMap, fragment_TexCoord).rgb - 1.f): fragment_Normal;
        float intencity = modelUseSpecularMap? texture(modelSpecularMap, fragment_TexCoord).r: 1.f;

        vec3 lightning_result = vec3(0, 0, 0);
        for (int i = 0; i < lightCount; i++)
            lightning_result += CalculatePointLight(lights[i], normal, viewPosition, fragment_Position, intencity);

        out_Color = source_color*vec4(lightning_result, 1.f);
    }

	else out_Color = source_color;
}

//--------------------------------- Lightning processing

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos, float intensity)
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
    vec3 specular = light.color.xyz*light.specular*spec*intensity;

    return ambient+diffuse+specular;
}

//---------------------------------