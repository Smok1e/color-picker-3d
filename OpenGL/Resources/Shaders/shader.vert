// Vertex shader
#version 450 core

//--------------------------------- Input

layout(location=0) in vec3 vertex_Position;
layout(location=1) in vec2 vertex_TexCoord;
layout(location=2) in vec3 vertex_Normal;

//layout(location=3) in vec3 vertex_Tangent;
//layout(location=4) in vec3 vertex_Bitangent;

//--------------------------------- Uniforms

uniform mat4 model;
uniform mat3 modelNormalMatrix;
uniform mat4 view;
uniform mat4 projection;

//--------------------------------- Output

out mat4 geometry_Model;
out vec3 geometry_Position;
out vec3 geometry_Normal;
out vec2 geometry_TexCoord;

//---------------------------------

void main()
{
    // Applying MVP matrix
    gl_Position = projection*view*model*vec4(vertex_Position, 1.f);

    // Passing values to geometry shader
    geometry_Model = model;
    geometry_Position = vertex_Position;
    geometry_Normal = modelNormalMatrix*vertex_Normal;
    geometry_TexCoord = vertex_TexCoord;
}

//---------------------------------