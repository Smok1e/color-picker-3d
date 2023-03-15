#version 330 core

//---------------------------------

uniform float ambientLightningStrength;
uniform vec3 lightPos;
uniform vec4 lightColor;

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
    vec3 diffuse = max(dot(vertexNormal, lightDirection), 0.0) * vec3(lightColor);

    color = vec4(ambient+diffuse, 1)*vertexColor;
}

//---------------------------------