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

// Ƭ����ɫ��
#type fragment
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_Texture;

const float offset = 1.0 / 300.0;

void main()
{
	// vec3 col = texture(u_Texture, texCoord).rgb;
	// FragColor = vec4(col, 1.0);

	// ���� ����Ļ������ȡ��ɫֵ��Ȼ����1.0��ȥ�����������з��ࣺ
	// FragColor = vec4(vec3(1.0 - texture(u_Texture, texCoord)), 1.0);

//	// �Ҷ� ȡ���е���ɫ������������ƽ������
//	FragColor = texture(u_Texture, texCoord);
//	// float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
//	// ���Ѿ��ܴ���ܺõĽ���ˣ������ۻ����ɫ��������һЩ��������ɫ����ô���У�����Ϊ�˻�ȡ�����ϸ���ȷ��Ч����������Ҫʹ�ü�Ȩ��(Weighted)ͨ����
//	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//	FragColor = vec4(average, average, average, 1.0);

	// �� �������
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

    // �񻯾��
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    // ģ�����
//    float kernel[9] = float[](
//        1.0 / 16, 2.0 / 16, 1.0 / 16,
//        2.0 / 16, 4.0 / 16, 2.0 / 16,
//        1.0 / 16, 2.0 / 16, 1.0 / 16  
//    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_Texture, texCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);

}
