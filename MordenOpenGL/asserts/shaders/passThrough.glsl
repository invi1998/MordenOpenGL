#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;

void main()
{
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0f, 1.0f);
}

// 几何着色器
#type geometry
#version 330 core

layout (points) in;
layout (points, max_vertices = 1) out;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
}


// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
