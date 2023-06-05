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

// 片段着色器
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

	// 反相 从屏幕纹理中取颜色值，然后用1.0减去它，对它进行反相：
	// FragColor = vec4(vec3(1.0 - texture(u_Texture, texCoord)), 1.0);

//	// 灰度 取所有的颜色分量，将它们平均化：
//	FragColor = texture(u_Texture, texCoord);
//	// float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
//	// 这已经能创造很好的结果了，但人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以为了获取物理上更精确的效果，我们需要使用加权的(Weighted)通道：
//	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//	FragColor = vec4(average, average, average, 1.0);

	// 核 （卷积）
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    // 锐化卷积
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    // 模糊卷积
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
