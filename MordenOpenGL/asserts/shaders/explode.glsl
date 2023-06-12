#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	vs_out.TexCoords = a_TexCoords;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

// ������ɫ��
#type geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
} gs_in[];

uniform float time;

out vec2 TexCoords;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 200.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 1.0);
}

vec3 GetNormal()
{
    // ��Ϊ������Ҫ���������εķ�����λ��ÿ�����㣬����������Ҫ���ȼ���������������
    // ����Ҫ���ľ��Ǽ��㴹ֱ�������α������������ʹ�������ܹ����ʵ��������㡣
    // ����ʹ�ò������ȡ��ֱ����������������һ�������������ܹ���ȡ����ƽ���������α��������a��b�����Ǿ��ܶ��������������в������ȡ��������
    // ��ʵ˵���ˣ�a��b������������ε������ߣ�Ȼ��ʹ�ò�˾��ܼ������ֱ������������ɵ�ƽ���һ����������������Ҫ���������淨������
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main()
{

    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}


// Ƭ����ɫ��
#type fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}
