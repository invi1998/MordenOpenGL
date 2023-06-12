#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out VS_OUT
{
    vec3 normal;
} vs_out;

uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.normal = vec3(vec4(normalMatrix * a_Normal, 0.0));

    gl_Position =  u_View * u_Model * vec4(a_Pos, 1.0);
}

// 几何着色器
#type geometry
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 u_Projection;

void GenerateLine(int index)
{
    gl_Position = u_Projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_Projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{

    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}


// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.31, 0.93, 0.1, 1.0);
}
