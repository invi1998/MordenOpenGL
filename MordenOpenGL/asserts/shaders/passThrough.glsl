#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;

void main()
{
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0f, 1.0f);
}

// ������ɫ��
#type geometry
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void build_house(vec4 position)
{    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:����
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:����
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

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
