// Fragment shader
#version 450 core

//--------------------------------- Lights

#define LIGHTS_MAX 16

struct PointLight
{
    vec3 position;
    vec4 color;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};

struct DirectionalLight
{
    vec3 direction;
    vec4 color;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec4 color;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
    float cutoff_angle;
    float outer_cutoff_angle;
};

//--------------------------------- Output

layout(location=0) out vec4 out_Color;

//--------------------------------- Uniforms

// Material
uniform sampler2D materialDiffuseMap;
uniform sampler2D materialNormalMap;
uniform sampler2D materialSpecularMap;
uniform sampler2D materialDepthMap;
uniform vec2      materialTextureScale;
uniform bool      materialUseDiffuseMap;
uniform bool      materialUseNormalMap;
uniform bool      materialUseSpecularMap;
uniform bool      materialUseDepthMap;
uniform vec4      materialColor;
uniform bool      materialUseLightning;

// Lights
uniform vec3             viewPosition;
uniform PointLight       pointLights[LIGHTS_MAX];
uniform int              pointLightCount;
uniform DirectionalLight dirLights[LIGHTS_MAX];
uniform int              dirLightCount;
uniform SpotLight        spotLights[LIGHTS_MAX];
uniform int              spotLightCount;

//--------------------------------- Input

in vec3 fragment_Position;
in vec3 fragment_Normal;
in mat3 fragment_TBN;
in vec2 fragment_TexCoord;

//--------------------------------- 

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewpos, vec3 fragpos, float specular_intensity);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewdir, float specular_intensity);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragpos, vec3 viewdir, float specular_intensity);

//--------------------------------- 

void main()
{
    vec2 texcoord = fragment_TexCoord*materialTextureScale;
    vec4 source_color = materialUseDiffuseMap? texture(materialDiffuseMap, texcoord): materialColor;

    if (materialUseLightning)
    {
        vec3 normal = materialUseNormalMap? fragment_TBN * normalize(2.f * texture(materialNormalMap, texcoord).rgb - 1.f): fragment_Normal;
        vec3 viewdir = normalize(viewPosition-fragment_Position);
        float specular_intensity = materialUseSpecularMap? texture(materialSpecularMap, texcoord).r: 1.f;

        vec3 lightning_result = vec3(0, 0, 0);

        // Point lights
        for (int i = 0; i < pointLightCount; i++)
            lightning_result += CalculatePointLight(pointLights[i], normal, viewPosition, fragment_Position, specular_intensity);

        // Directional lights
        for (int i = 0; i < dirLightCount; i++)
            lightning_result += CalculateDirectionalLight(dirLights[i], normal, viewdir, specular_intensity);

        // Spot lights
        for (int i = 0; i < spotLightCount; i++)
            lightning_result += CalculateSpotLight(spotLights[i], normal, fragment_Position, viewdir, specular_intensity);

        out_Color = source_color*vec4(lightning_result, 1.f);
    }

	else out_Color = source_color;
}

//--------------------------------- Lightning processing

// Point light
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewpos, vec3 fragpos, float specular_intensity)
{
    // Ambient
    vec3 ambient = light.color.rgb*light.ambient_strength;

    // Diffuse
    vec3 lightdir = normalize(light.position-fragpos);
    float diff = max(dot(normal, lightdir), .0f);
    vec3 diffuse = light.color.rgb*light.diffuse_strength*diff;

    // Specular
    vec3 viewdir = normalize(viewpos-light.position);
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectdir), .0f), 32);
    vec3 specular = light.color.rgb*light.specular_strength*spec*specular_intensity;

    return ambient+diffuse+specular;
}

// Directional light
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewdir, float specular_intensity)
{
    // Ambient
    vec3 ambient = light.color.rgb*light.ambient_strength;

    // Diffuse
    vec3 lightdir = normalize(-light.direction);
    float diff = max(dot(normal, lightdir), .0f);
    vec3 diffuse = light.color.xyz*light.diffuse_strength*diff;

    // Specular
    vec3 reflectDir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectDir), .0f), 32);
    vec3 specular = light.color.xyz*light.specular_strength*spec*specular_intensity;

    return ambient+diffuse+specular; 
}

// Spot light
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragpos, vec3 viewdir, float specular_intensity)
{
    // Ambient
    vec3 ambient = light.color.rgb*light.ambient_strength;

    vec3 lightdir = normalize(light.position-fragpos);
    float theta = dot(lightdir, normalize(-light.direction));  

    float epsilon = light.cutoff_angle - light.outer_cutoff_angle;
    float intensity = -clamp((theta-light.outer_cutoff_angle)/epsilon, -1.f, 0.f);
    if (theta <= light.cutoff_angle)
        return ambient;

    // Diffuse
    float diff = max(dot(normal, lightdir), .0f);
    vec3 diffuse = light.color.xyz*light.diffuse_strength*diff*intensity;

    // Specular
    vec3 reflectDir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectDir), .0f), 32);
    vec3 specular = light.color.xyz*light.specular_strength*spec*specular_intensity*intensity;
        
    return ambient+diffuse+specular;
}

//---------------------------------