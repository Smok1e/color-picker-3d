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
uniform bool      modelUseTexture;
uniform bool      modelUseLightning;

uniform vec3      viewPosition;

uniform Light     lights[LIGHTS_MAX];
uniform int       lightCount;

//--------------------------------- Input

in vec3 fragment_Position;
in vec3 fragment_Normal;
in vec2 fragment_TexCoord;

//--------------------------------- 

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos);

//--------------------------------- 

void main()
{
    vec4 source_color = modelUseTexture? texture(modelTexture, fragment_TexCoord): modelColor;
    if (modelUseLightning)
    {
        vec3 lightning_result = vec3(0, 0, 0);
        for (int i = 0; i < lightCount; i++)
            lightning_result += CalculatePointLight(lights[i], fragment_Normal, viewPosition, fragment_Position);

        out_Color = source_color*vec4(lightning_result, 1.f);
    }

	else out_Color = source_color;
}

//--------------------------------- Lightning processing

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