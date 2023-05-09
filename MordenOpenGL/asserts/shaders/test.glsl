#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);
	vertexColor = a_Pos;
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

in vec3 vertexColor;

void main()
{
	FragColor = vec4(vertexColor * 0.9 + 0.6, 1.0f);
}
