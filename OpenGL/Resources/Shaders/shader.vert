// Vertex shader
#version 450 core

//--------------------------------- Input

layout(location=0) in vec3 vertex_Position;
layout(location=1) in vec2 vertex_TexCoord;
layout(location=2) in vec3 vertex_Normal;
layout(location=3) in vec3 vertex_Tangent;

//--------------------------------- Uniforms

uniform mat4 model;
uniform mat3 modelNormalMatrix;
uniform mat4 view;
uniform mat4 projection;

//--------------------------------- Output

smooth out vec3 geometry_Position;
smooth out vec3 geometry_Normal;
       out mat3 geometry_TBN;
smooth out vec2 geometry_TexCoord;

//---------------------------------

void main()
{
    // Applying MVP matrix
    gl_Position = projection*view*model*vec4(vertex_Position, 1.f);

    // Calculating the tangent space
    vec3 normal = modelNormalMatrix*normalize(vertex_Normal);
    vec3 tangent = modelNormalMatrix*normalize(vertex_Tangent);
    vec3 bitangent = cross(normal, tangent);

    // Passing values to geometry shader
    geometry_Position = (model*vec4(vertex_Position, 1.f)).xyz;
    geometry_Normal = normal;
    geometry_TBN = mat3(tangent, bitangent, normal);
    geometry_TexCoord = vertex_TexCoord;
}

//---------------------------------