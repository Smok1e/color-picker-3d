#version 330 core

//---------------------------------

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

uniform vec4 shapeColor;
uniform vec3 shapeOffset;

out vec4 vertexColor;

//---------------------------------

void main ()
{
    gl_Position = projection * view * vec4(position+shapeOffset, 1.0f);
    vertexColor = shapeColor; 
}

//---------------------------------