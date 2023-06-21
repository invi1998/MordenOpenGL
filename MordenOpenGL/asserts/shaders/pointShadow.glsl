#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_Model;

void main()
{
	// ������ɫ��������������ռ�Ķ���任��6����ͬ�Ĺ�ռ����ɫ����
	// ��˶�����ɫ���򵥵Ľ�����任������ռ䣬Ȼ�󷢸�������ɫ��
	gl_Position = u_Model * vec4(a_Pos, 1.0f);
}

// ������ɫ��
#type geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;	// �Ӽ�����ɫ���л�ȡ�� FragPos��ÿ�� EmitVertex �����

// ������ɫ����Լ򵥡���������һ�������Σ�����ܹ�6�������Σ�6*3���㣬�����ܹ�18�����㣩��
// ��main�����У����Ǳ�����������ͼ��6���棬����ÿ����ָ��Ϊһ������棬��������interger���������浽gl_Layer��
// Ȼ������ͨ������Ĺ�ռ�任�������FragPos����ÿ������ռ䶥��任����صĹ�ռ䣬����ÿ�������Ρ�
// ע�⣬���ǻ�Ҫ������FragPos�������͸�������ɫ����������Ҫ����һ�����ֵ��
void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;	// ���ñ�����ָ������Ҫ��Ⱦ���ĸ��档
		for (int i = 0;  i < 3;  ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}


// Ƭ����ɫ��
#type fragment
#version 330 core

in vec4 FragPos;

uniform vec3 u_LightPos;;

uniform float farPlane;

// ���ǽ������Լ�����ȣ������Ⱦ���ÿ��fragmentλ�ú͹�Դλ��֮������Ծ��롣�����Լ������ֵʹ��֮�����Ӱ�������ֱ��

void main()
{
	// ��ȡƬ�ε���Դ�ľ���
	float lightDistance = length(FragPos.xyz - u_LightPos);

	// ������ӳ�䵽 [0;1] ��Χ�ڣ�ͨ������Զƽ�����ʵ��
	lightDistance = lightDistance / farPlane;

	// ��������ľ�����Ϊ�޸ĺ�����ֵ����д��
	gl_FragDepth = lightDistance;
}
