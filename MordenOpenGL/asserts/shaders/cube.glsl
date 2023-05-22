#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out vec3 FragPos;
out vec3 Normal;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{

	FragPos = vec3(u_Model * vec4(a_Pos, 1.0f));

	Normal = a_Normal;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0f);
}

// Ƭ����ɫ��
#type fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_LightPos;	// ����λ��
uniform vec3 u_LightColor;	// ������ɫ
uniform vec3 u_ObjectColor;	// ������ɫ

void main()
{
	// ������
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

	// ������
	vec3 norm = normalize(Normal);						// ������һ��
	vec3 lightDir = normalize(u_LightPos - FragPos);	// �����Դ������ķ�����������һ�������������䷽��
	float diff = max(dot(norm, lightDir), 0.0f);		// �ù��߷���Ͷ��㷨������ˣ���˽�����Ǿ����������ߵļнǣ����ߺͶ��㷨�ߣ�������нǾͱ�ʾ�˹�Դ�Ե�ǰ�����ʵ��������Ч��
	// ���������ʱ����ͨ��������һ��������ģ��������λ�ã�����ֻ�������ǵķ������ԣ��������еļ��㶼ʹ�õ�λ������ɣ���Ϊ����˴󲿷ֵļ��㣨�����ˣ���
	// ���Ե����й��ռ���ʱ��ȷ�������Ƕ�����������б�׼��������֤�����������ص�λ���������Ƕ��������б�׼����һ��ʮ�ֳ����Ĵ���
	// ͬʱȷ��������ֵȡ����0��ֵ����ΪС��0��ʾ�Ĺ�����û�������
	vec3 diffuse = diff * u_LightColor;

	vec3 result = (ambient + diffuse) * u_ObjectColor;
	FragColor = vec4(result, 1.0f);

}
