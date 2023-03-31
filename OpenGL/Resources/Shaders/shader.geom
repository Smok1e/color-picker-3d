// Geometry shader
#version 450 core

//--------------------------------- Attributes

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

//--------------------------------- Input

in mat4 geometry_Model[];
in vec3 geometry_Position[];
in vec3 geometry_Normal[];
in vec2 geometry_TexCoord[];

//--------------------------------- Output

smooth out vec3 fragment_Position;
smooth out vec3 fragment_Normal;
smooth out vec2 fragment_TexCoord;

//--------------------------------- 

void main()
{
    // Processing primitive
    for (int i = 0, count = gl_in.length(); i < count; i++)
    {
        gl_Position = gl_in[i].gl_Position;

        // Passing values to fragment shader
        fragment_Position = (geometry_Model[i]*vec4(geometry_Position[i], 1.f)).xyz;
        fragment_Normal = geometry_Normal[i];
        fragment_TexCoord = geometry_TexCoord[i];
        EmitVertex();
    }

    EndPrimitive();
}

//---------------------------------