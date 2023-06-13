#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_TexCoords;

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

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_DiffuseTexture1;

void main()
{
	FragColor = texture(u_DiffuseTexture1, TexCoords);
}
