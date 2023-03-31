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

//--------------------------------- Uniforms

uniform bool modelUseNormalCalculation;

//--------------------------------- Output

smooth out vec3 fragment_Position;
smooth out vec3 fragment_Normal;
smooth out vec2 fragment_TexCoord;

//--------------------------------- 

void main()
{
    // Calculating normal
    vec3 normal;
    if (modelUseNormalCalculation)
    {
        vec3 edge1 = geometry_Position[2]-geometry_Position[0];
        vec3 edge2 = geometry_Position[1]-geometry_Position[0];
        normal = normalize((inverse(geometry_Model[0])*vec4(cross(edge1, edge2), 1.f)).xyz);
    }

    // Processing primitive
    for (int i = 0, count = gl_in.length(); i < count; i++)
    {
        gl_Position = gl_in[i].gl_Position;

        // Passing values to fragment shader
        fragment_Position = (geometry_Model[i]*vec4(geometry_Position[i], 1.f)).xyz;
        fragment_Normal = modelUseNormalCalculation? normal: geometry_Normal[i];
        fragment_TexCoord = geometry_TexCoord[i];
        EmitVertex();
    }

    EndPrimitive();
}

//---------------------------------