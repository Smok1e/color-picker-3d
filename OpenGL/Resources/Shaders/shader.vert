#version 330 core

//---------------------------------

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 shapeOffset;
uniform vec3 shapeScale;
uniform mat4 shapeTransform;
uniform vec4 shapeColor;

out vec4 vertexColor;

//---------------------------------

void main ()
{
    gl_Position = vec4(shapeOffset, 0) + projection * view * shapeTransform * vec4(position*shapeScale, 1.0f);
    vertexColor = shapeColor; 
}

//---------------------------------