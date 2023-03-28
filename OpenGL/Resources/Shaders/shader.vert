#version 450 core

//---------------------------------

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 shapeOffset;
uniform vec3 shapeScale;
uniform mat4 shapeTransform;
uniform vec4 shapeColor;

uniform bool      useBumpMap;
uniform sampler2D shapeBumpMap;

out vec3 FragPos;
out vec4 FragColor;
out vec2 FragTexCoord;
out vec3 FragNormal;
out mat3 FragTBN;

//---------------------------------

void main ()
{
    vec3 bump = useBumpMap? normal*vec3(texture(shapeBumpMap, texcoord))*0.1: vec3(0);
    vec4 modelVertexPosition = vec4((bump+position)*shapeScale, 1.0f);

    vec4 offset = vec4(shapeOffset, 0);
    gl_Position = projection * view * (offset + shapeTransform * modelVertexPosition);

    FragPos = vec3(offset + modelVertexPosition);
    FragColor = shapeColor; 
    FragNormal = mat3(transpose(inverse(shapeTransform)))*normal;
    FragTexCoord = texcoord;

    FragTBN = mat3(
        normalize(vec3(shapeTransform*vec4(tangent,   0))),
        normalize(vec3(shapeTransform*vec4(bitangent, 0))),
        normalize(vec3(shapeTransform*vec4(normal,    0)))
    );
}

//---------------------------------