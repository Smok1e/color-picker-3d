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

// Material
uniform sampler2D materialDiffuseMap;
uniform sampler2D materialNormalMap;
uniform sampler2D materialSpecularMap;
uniform sampler2D materialDepthMap;
uniform bool      materialUseDiffuseMap;
uniform bool      materialUseNormalMap;
uniform bool      materialUseSpecularMap;
uniform bool      materialUseDepthMap;
uniform vec4      materialColor;
uniform bool      materialUseLightning;

// Lightning
uniform vec3      viewPosition;
uniform Light     lights[LIGHTS_MAX];
uniform int       lightCount;

//--------------------------------- Input

smooth in vec3 fragment_Position;
smooth in vec3 fragment_Normal;
       in mat3 fragment_TBN;
smooth in vec2 fragment_TexCoord;

//--------------------------------- 

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos, float specular_intensity);

//--------------------------------- 

void main()
{
    vec4 source_color = materialUseDiffuseMap? texture(materialDiffuseMap, fragment_TexCoord): materialColor;

    if (materialUseLightning)
    {
        vec3 normal = materialUseNormalMap? fragment_TBN * normalize(2.f * texture(materialNormalMap, fragment_TexCoord).rgb - 1.f): fragment_Normal;
        float specular_intensity = materialUseSpecularMap? texture(materialSpecularMap, fragment_TexCoord).r: 1.f;

        vec3 lightning_result = vec3(0, 0, 0);
        for (int i = 0; i < lightCount; i++)
            lightning_result += CalculatePointLight(lights[i], normal, viewPosition, fragment_Position, specular_intensity);

        out_Color = source_color*vec4(lightning_result, 1.f);
    }

	else out_Color = source_color;
}

//--------------------------------- Lightning processing

vec3 CalculatePointLight(Light light, vec3 normal, vec3 viewpos, vec3 fragpos, float specular_intensity)
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
    vec3 specular = light.color.xyz*light.specular*spec*specular_intensity;

    return ambient+diffuse+specular;
}

//---------------------------------