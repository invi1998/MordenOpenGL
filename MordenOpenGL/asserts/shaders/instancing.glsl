// 顶点着色器
#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec3 a_Color;

out vec3 fColor;

uniform vec2 u_Offsets[100];

void main()
{
	vec2 offset = u_Offsets[gl_InstanceID];
	gl_Position = vec4(a_Pos + offset, 0.0, 1.0);
	fColor = a_Color;
}

// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

in vec3 fColor;

void main()
{
	FragColor = vec4(fColor, 1.0);
}
