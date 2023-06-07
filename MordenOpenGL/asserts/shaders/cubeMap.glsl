#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

// out vec3 vertexColor;
out vec3 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	TexCoords = a_Pos;
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0f);
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_Texture;

void main()
{
	FragColor = texture(u_Texture, TexCoords);
}
