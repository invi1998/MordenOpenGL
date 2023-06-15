#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform bool u_ReverseNormals;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
	vs_out.FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
	if (u_ReverseNormals)
	{
		vs_out.Normal = transpose(inverse(mat3(u_Model))) * (-1.0 * a_Normal);
	}
	else
	{
		vs_out.Normal = transpose(inverse(mat3(u_Model))) * a_Normal;
	}
	vs_out.TexCoords = a_TexCoords;
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

uniform sampler2D u_DiffuseTexture;
uniform samplerCube u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

uniform float farPlane;
uniform bool shadows;

float ShadowCalculation(vec3 fragPos)
{
	// 获取片段位置与光源位置之间的向量。
	vec3 fragToLight = fragPos - u_LightPos;

	// 使用片段到光源的向量从深度图中进行采样。
	float closestDepth = texture(u_DepthMap, fragToLight).r;

	// 深度值当前处于 [0, 1] 的线性范围内。让我们将其重新转换回原始深度值。
	closestDepth *= farPlane;

	// 现在，将当前的线性深度作为片段到光源位置的长度。
	float currentDepth = length(fragToLight);

	// 现在进行阴影测试。
	float bias = 0.05;	// 由于深度现在处于 [近平面，远平面] 范围内，所以我们使用一个更大的偏差。
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;

}

void main()
{
	vec3 color = texture(u_DiffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);
	// 环境光
	vec3 ambient = 0.15 * color;
	// 漫反射光
	vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// 高光
	vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	// 计算阴影
	float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}
