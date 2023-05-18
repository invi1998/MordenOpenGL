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
	// vertexColor = a_Pos * 0.9 + 0.6;
	texCoord = vec2(a_TexCoord.x, a_TexCoord.y);
	// gl_Position = u_Transform * vec4(a_Pos, 1.0f);
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core

out vec4 FragColor;

// in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	// FragColor = vec4(texCoord, 0, 1.0);
	// FragColor = mix(texture(u_Texture1, texCoord), texture(u_Texture2, texCoord), 0.5);
	// FragColor = texture(u_Texture1, texCoord);
	FragColor = texture(u_Texture1, texCoord) + texture(u_Texture2, texCoord);
	// vec4 color = texture(u_Texture2, texCoord);
	//	if (color.a <= 0.1)
	//	{
	//		discard;
	//	}
	// FragColor += color;
}
