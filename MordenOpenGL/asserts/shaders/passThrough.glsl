#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec3 a_Color;

out VS_OUT
{
    vec3 color;
} vs_out;

void main()
{
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0f, 1.0f);
    vs_out.color = a_Color;
}

// ������ɫ��
#type geometry
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 f_Color;

void build_house(vec4 position)
{    
    f_Color = gs_in[0].color;   // gs_in[0] ��Ϊֻ��һ�����붥��
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:����
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:����
    f_Color = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
	build_house(gl_in[0].gl_Position);
}


// Ƭ����ɫ��
#type fragment
#version 330 core

in vec3 f_Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(f_Color, 1.0);
}
