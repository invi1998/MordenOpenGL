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

void main()
{
	// vec3 col = texture(u_Texture, texCoord).rgb;
	// FragColor = vec4(col, 1.0);

	// 反相 从屏幕纹理中取颜色值，然后用1.0减去它，对它进行反相：
	// FragColor = vec4(vec3(1.0 - texture(u_Texture, texCoord)), 1.0);

	// 灰度 取所有的颜色分量，将它们平均化：
	FragColor = texture(u_Texture, texCoord);
	// float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	// 这已经能创造很好的结果了，但人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以为了获取物理上更精确的效果，我们需要使用加权的(Weighted)通道：
	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	FragColor = vec4(average, average, average, 1.0);

}
