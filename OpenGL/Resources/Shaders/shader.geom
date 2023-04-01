// Geometry shader
#version 450 core

//--------------------------------- Attributes

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

//--------------------------------- Input

smooth in vec3 geometry_Position[];
smooth in vec3 geometry_Normal[];
       in mat3 geometry_TBN[];
smooth in vec2 geometry_TexCoord[];

//--------------------------------- Output

smooth out vec3 fragment_Position;
smooth out vec3 fragment_Normal;
       out mat3 fragment_TBN;
smooth out vec2 fragment_TexCoord;

//--------------------------------- 

void main()
{
    // Processing primitive
    for (int i = 0, count = gl_in.length(); i < count; i++)
    {
        gl_Position = gl_in[i].gl_Position;

        // Passing values to fragment shader
        fragment_Position = geometry_Position[i];
        fragment_Normal = geometry_Normal[i];
        fragment_TBN = geometry_TBN[i];
        fragment_TexCoord = geometry_TexCoord[i];
        EmitVertex();
    }

    EndPrimitive();
}

//---------------------------------