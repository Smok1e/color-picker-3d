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
    color = vertexColor;
}

//---------------------------------