// 绘制边
// 顶点着色器
#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 TexCoords;

void main()
{
	TexCoords = a_TexCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

// in vec2 TexCoords;

void main()
{
	FragColor = vec4(0.02, 0.89, 0.13, 1.0);
}
