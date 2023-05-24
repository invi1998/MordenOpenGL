#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;		// ����λ��
layout (location = 1) in vec3 a_Normal;		// ���㷨��
layout (location = 2) in vec2 a_TexCoords;		// ��������

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{

	FragPos = vec3(u_Model * vec4(a_Pos + 0.5f, 1.0f));

	// ���߾��󱻶���Ϊ��ģ�;������Ͻ�3x3���ֵ�������ת�þ���
	// ÿ������Ӧ��һ�����ȱ�����ʱ��ע�⣺�ȱ����Ų����ƻ����ߣ���Ϊ���ߵķ���û���ı䣬�����ı��˷��ߵĳ��ȣ����������ͨ����׼�����޸������������Ͳ����ٴ�ֱ�ڶ�Ӧ�ı����ˣ��������վͻᱻ�ƻ���

	// �޸������Ϊ�ľ�����ʹ��һ��Ϊ������ר�Ŷ��Ƶ�ģ�;�����������֮Ϊ���߾���(Normal Matrix)����ʹ����һЩ���Դ����Ĳ������Ƴ��Է������������ŵ�Ӱ�졣
	// �ڶ�����ɫ���У����ǿ���ʹ��inverse��transpose�����Լ�����������߾����������������������;�����Ч��
	// ע�����ǻ�Ҫ�ѱ�������ľ���ǿ��ת��Ϊ3��3��������֤��ʧȥ��λ�������Լ��ܹ�����vec3�ķ�������
	Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	TexCoords = a_TexCoords;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0f);
}

// Ƭ����ɫ��
#type fragment
#version 330 core

// ��������
//struct Material
//{
//	vec3 ambient;		// ��������
//	vec3 diffuse;		// ���������
//	vec3 specular;		// �������
//	float shininess;	// �����
//};

// ������ͼ��������
struct Material
{
	sampler2D diffuse;		// ��������ͼ
	sampler2D specular;		// ���淴����ͼ
	float shininess;		// �����
};

// ��������
struct Light
{
	vec3 position;		// ��Դλ��

	vec3 ambient;		// ��������ǿ��
	vec3 diffuse;		// ���������ǿ��
	vec3 specular;		// �������ǿ��

	float constant;		// ������
    float linear;		// һ����
    float quadratic;	// ������
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 u_ViewPos;		// �����λ��

uniform Material u_Material;	// ����
uniform Light u_Light;			// ����

void main()
{
	// ��Դ�Ͷ���ľ���
	float distances = length(u_Light.position - FragPos);
	// ����˥��ֵ
	float attenuation = 1.0f / (u_Light.constant + u_Light.linear * distances + u_Light.quadratic * (distances * distances));

	// ������
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoords).rgb;

	// ������
	vec3 norm = normalize(Normal);						// ������һ��
	vec3 lightDir = normalize(u_Light.position - FragPos);	// �����Դ������ķ�����������һ�������������䷽��
	float diff = max(dot(norm, lightDir), 0.0f);		// �ù��߷���Ͷ��㷨������ˣ���˽�����Ǿ����������ߵļнǣ����ߺͶ��㷨�ߣ�������нǾͱ�ʾ�˹�Դ�Ե�ǰ�����ʵ��������Ч��
	// ���������ʱ����ͨ��������һ��������ģ��������λ�ã�����ֻ�������ǵķ������ԣ��������еļ��㶼ʹ�õ�λ������ɣ���Ϊ����˴󲿷ֵļ��㣨�����ˣ���
	// ���Ե����й��ռ���ʱ��ȷ�������Ƕ�����������б�׼��������֤�����������ص�λ���������Ƕ��������б�׼����һ��ʮ�ֳ����Ĵ���
	// ͬʱȷ��������ֵȡ����0��ֵ����ΪС��0��ʾ�Ĺ�����û�������
	vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoords).rgb;		// �������ʵ����ͼ��Ⱦ��������ͻ��������Ⱦ����Ϊ��������ɫ�ڼ�����������¶�������������ɫ����������ʹ��ͬһ�����Խ��м���

	// �������
	vec3 viewDir = normalize(u_ViewPos - FragPos);	// ���߷�������
	vec3 reflectDir = reflect(-lightDir, norm);		// ���ŷ�����ķ�������
	// ��Ҫע��������Ƕ�lightDir����������ȡ����
	// reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴���Ǵ�Ƭ��ָ���Դ������ǰ���Ǽ���lightDir����ʱ��������˳���������
	// Ϊ�˱�֤���ǵõ���ȷ��reflect����������ͨ����lightDir����ȡ��������෴�ķ��򡣵ڶ�������Ҫ����һ�������������������ṩ�����ѱ�׼����norm������
	// ���㾵�����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);	// �����ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ����32�Ǹ߹�ķ����(Shininess)��
	// һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС��
	vec3 specular = u_Light.specular * spec * texture(u_Material.specular, TexCoords).rgb; // ���һ�������ǰ����ӵ�������������������������ý�������������ɫ

	// ���߼�����Ͼ���˥��ֵ
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);

	// FragColor = texture(u_Material.specular, TexCoords);

}
