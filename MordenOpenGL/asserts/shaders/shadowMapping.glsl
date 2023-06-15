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
	// ������u_LightSpaceMatrix��������ռ䶥��λ��ת���ɹ�ռ䣬������ɫ������һ����ͨ�ľ����任������ռ䶥��λ��vs_out.FragPos��һ����ռ��vs_out.FragPosLightSpace��Ƭ����ɫ��
	vs_out.FragPosLightSpece = u_LightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}

// Ƭ����ɫ��
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
	// ִ��͸�ӳ���
	// ������Ƭ���ڹ�ռ��-1��1֮��ķ�Χ
	// ��ʹ������ͶӰ���󣬶���wԪ����Ȼ���ֲ��䣬������һ��ʵ���Ϻ������壬���ǣ���ʹ��͸��ͶӰ�����ʱ�򣬾��Ǳ�����ˣ�����Ϊ�˱�֤������ͶӰ�����¶���Ч����һ�б����ž���
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// ��Ϊ���������ͼ�����ֵ�ڡ�0,1��֮�䣬����Ҳ������projCoordsȥ�����ͼ�в�����������Ҫ��NDC����תΪ0��1�ķ�Χ
	projCoords = projCoords * 0.5 + 0.5;

	// ��ȡ�ӹ��λ����������
	float closestDepth = texture(u_ShaderMap, projCoords.xy).r;

	// ��ȡͶӰ������z���꣬���������Թ��͸�ӽǵ�Ƭ�ε����
	float currentDepth = projCoords.z;

	// �򵥼��currentDepth�Ƿ����closetDepth������ǣ���ôƬ�ξ�����Ӱ�С�
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;

}

void main()
{
	vec3 color = texture(u_DiffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);
	// ������
	vec3 ambient = 0.15 * color;
	// �������
	vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// �߹�
	vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	// ������Ӱ
	float shadow = ShadowCalculation(fs_in.FragPosLightSpece);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}
