#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TexCoord;


// out vec3 vertexColor;
out vec2 texCoord;

void main()
{
	texCoord = a_TexCoord;
	gl_Position = vec4(a_Pos, 0.0, 1.0);
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_Texture;

void main()
{
	vec3 col = texture(u_Texture, texCoord).rgb;
	// FragColor = vec4(texCoord, 0.0, 1.0);
	FragColor = vec4(col, 1.0);
}
