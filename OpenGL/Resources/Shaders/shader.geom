#version 450 core

//---------------------------------

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 GeoPos[];
in vec4 GeoColor[];
in vec2 GeoTexCoord[];
in vec3 GeoNormal[];
in mat3 GeoTBN[];

out vec3 FragPos;
out vec4 FragColor;
out vec2 FragTexCoord;
out vec3 FragNormal;
out mat3 FragTBN;

//---------------------------------

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        FragPos      = GeoPos     [i];
        FragColor    = GeoColor   [i];
        FragTexCoord = GeoTexCoord[i];
        FragNormal   = GeoNormal  [i];
        FragTBN      = GeoTBN     [i];
        EmitVertex();
    }
    EndPrimitive();
}

//---------------------------------