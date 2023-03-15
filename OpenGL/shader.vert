#version 330 core

//---------------------------------

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;

out vec4 vertexColor;
out vec3 vertexNormal;
out vec3 fragPos;

//---------------------------------

void main ()
{
    gl_Position = projection * view * vec4(position+offset, 1.0f);
    fragPos = vec3(projection * vec4(position+offset, 1.0f));
    vertexColor = color; 
    vertexNormal = normal;
}

//---------------------------------