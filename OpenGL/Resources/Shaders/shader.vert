#version 330 core

//---------------------------------

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 shapeOffset;
uniform vec3 shapeScale;
uniform mat4 shapeTransform;
uniform vec4 shapeColor;

out vec3 FragPos;
out vec4 FragColor;
out vec2 FragTexCoord;
out vec3 FragNormal;

//---------------------------------

void main ()
{
    vec4 modelVertexPosition = vec4(position*shapeScale, 1.0f); 

    vec4 offset = vec4(shapeOffset, 0);
    gl_Position = projection * view * (offset + shapeTransform * vec4(position*shapeScale, 1.0f));

    FragPos = vec3(offset + shapeTransform*vec4(position, 0));
    FragColor = shapeColor; 
    FragNormal = mat3(transpose(inverse(shapeTransform)))*normal;
    FragTexCoord = texcoord;
}

//---------------------------------