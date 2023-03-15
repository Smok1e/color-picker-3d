#version 330 core

//---------------------------------

uniform float ambientLightningStrength;
uniform float diffuseLightningStrength;
uniform float specularLightningStrength;

uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec3 viewPos;

in vec3 vertexNormal;
in vec4 vertexColor;
in vec3 fragPos;

out vec4 color;

//---------------------------------

void main ()
{
    //vec3 lightColor = vec3(0, 0.7, 1);
    vec3 ambientColor = vec3(1, 1, 1);

    vec3 ambient = ambientLightningStrength*ambientColor;

    vec3 lightDirection = normalize(lightPos-fragPos);
    float diff = max(dot(vertexNormal, lightDirection), 0.0);
    vec3 diffuse =  diffuseLightningStrength * diff * vec3(lightColor);

    vec3 viewDirection = normalize(viewPos-fragPos);
    vec3 reflectionDirection = reflect(-lightDirection, vertexNormal);

    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
    vec3 specular = specularLightningStrength * spec * vec3(lightColor);

    color = vec4(ambient+diffuse+specular, 1)*vertexColor;
}

//---------------------------------