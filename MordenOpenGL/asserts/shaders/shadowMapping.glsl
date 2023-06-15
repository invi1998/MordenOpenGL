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
	vec4 FragPosLightSpece;
} vs_out;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
	vs_out.FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(u_Model))) * a_Normal;
	vs_out.TexCoords = a_TexCoords;
	// 我们用u_LightSpaceMatrix，把世界空间顶点位置转换成光空间，顶点着色器传递一个普通的经过变换的世界空间顶点位置vs_out.FragPos和一个光空间的vs_out.FragPosLightSpace给片段着色器
	vs_out.FragPosLightSpece = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
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
	vec4 FragPosLightSpece;
} fs_in;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_ShaderMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// 执行透视除法
	// 返回了片段在光空间的-1到1之间的范围
	// 当使用正交投影矩阵，顶点w元素任然保持不变，所以这一步实际上毫无意义，但是，当使用透视投影矩阵的时候，就是必须的了，所以为了保证在两种投影矩阵下都有效，这一行保留着就行
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// 因为来自深度贴图的深度值在【0,1】之间，我们也打算用projCoords去深度贴图中采样，所以需要将NDC坐标转为0到1的范围
	projCoords = projCoords * 0.5 + 0.5;

	// 获取从光的位置最近的深度
	float closestDepth = texture(u_ShaderMap, projCoords.xy).r;

	// 获取投影向量的z坐标，它等于来自光的透视角的片段的深度
	float currentDepth = projCoords.z;

	// 简单检查currentDepth是否高于closetDepth，如果是，那么片段就在阴影中。
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

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
	float shadow = ShadowCalculation(fs_in.FragPosLightSpece);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}
