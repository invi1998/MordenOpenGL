#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;


// out vec3 vertexColor;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Position;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	Position = vec3(u_Model * vec4(a_Pos, 1.0));
	TexCoords = a_TexCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 Position;


uniform sampler2D u_Texture;
uniform samplerCube u_SkyMap;
uniform vec3 u_CameraPos;
uniform vec2 u_ViewportSize;

void main()
{
	// FragColor = texture(u_Texture, TexCoords);
	// FragColor = vec4(texCoord, 0.0, 1.0);

	// 折射率
	float ratio = 1.00 / 1.52;

	// 观察向量
	vec3 I = normalize(Position-u_CameraPos);
	// 折射向量
	// 反射
	// vec3 R = reflect(I, normalize(Normal));
	// 折射
	vec3 R = refract(I, normalize(Normal), ratio);

	if (gl_FragCoord.x < (u_ViewportSize.x * 0.5))
	{
		FragColor = texture(u_Texture, TexCoords);
	}
	else
	{
		FragColor = vec4(texture(u_SkyMap, R).rgb, 1.0);
	}
}
