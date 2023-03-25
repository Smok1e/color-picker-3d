#version 330 core

//---------------------------------

in vec3 FragPos;
in vec4 FragColor;
in vec3 FragNormal;

out vec4 color;
uniform bool useTexture;

//---------------------------------

void main ()
{
    if (!useTexture)
        color = FragColor;
}

//---------------------------------