#version 330 core

//---------------------------------

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
out vec4 vertexColor;

//---------------------------------

void main ()
{
    gl_Position = projection * view * vec4(position+offset, 1.0f);
    vertexColor = color; 
}

//---------------------------------