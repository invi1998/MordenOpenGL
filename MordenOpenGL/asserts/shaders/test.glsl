#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoord;


// out vec3 vertexColor;
out vec2 texCoord;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	texCoord = a_TexCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, texCoord);
}
