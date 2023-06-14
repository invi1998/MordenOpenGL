#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

void main()
{
	vs_out.FragPos = a_Pos;
    vs_out.Normal = a_Normal;
    vs_out.TexCoords = a_TexCoords;
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D u_FloorTexture;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform bool u_Blinn;

void main()
{
	vec3 color = texture(u_FloorTexture, fs_in.TexCoords).rgb;
	// ambient 环境光
	vec3 ambient = 0.05 * color;
	// diffuse 漫反射
	vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular 高光
	vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);

	float spec = 0.0;
	if (u_Blinn)
	{
		// blinn phong 光照模型，采用半程向量和法线夹角判断高光
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	}
	else
	{
		// phong 光照模型
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}

	vec3 specular = vec3(0.3) * spec;
	FragColor = vec4(ambient + diffuse + specular, 1.0);
	
}
