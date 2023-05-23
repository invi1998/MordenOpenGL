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

	Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_LightPos;	// 光照位置
uniform vec3 u_ViewPos;		// 光照位置
uniform vec3 u_LightColor;	// 光照颜色
uniform vec3 u_ObjectColor;	// 物体颜色

void main()
{
	// 环境光
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

	// 漫反射
	vec3 norm = normalize(Normal);						// 向量归一化
	vec3 lightDir = normalize(u_LightPos - FragPos);	// 计算光源到顶点的方向向量并归一化，（光线入射方向）
	float diff = max(dot(norm, lightDir), 0.0f);		// 用光线方向和顶点法线做点乘，点乘结果就是就是两个射线的夹角（光线和顶点法线），这个夹角就表示了光源对当前顶点的实际漫反射效果
	// 当计算光照时我们通常不关心一个向量的模长或它的位置，我们只关心它们的方向。所以，几乎所有的计算都使用单位向量完成，因为这简化了大部分的计算（比如点乘）。
	// 所以当进行光照计算时，确保你总是对相关向量进行标准化，来保证它们是真正地单位向量。忘记对向量进行标准化是一个十分常见的错误。
	// 同时确保这个点乘值取大于0的值，因为小于0表示的光照是没有意义的
	vec3 diffuse = diff * u_LightColor;

	// 镜面光照
	float specularStrength = 0.8f;	// 镜面光照强度
	vec3 viewDir = normalize(u_ViewPos - FragPos);	// 视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);		// 沿着法线轴的反射向量
	// 需要注意的是我们对lightDir向量进行了取反。
	// reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
	// 为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。第二个参数要求是一个法向量，所以我们提供的是已标准化的norm向量。
	
	// 计算镜面分量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);	// 我们先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)。
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
	vec3 specular = specularStrength * spec * u_LightColor; // 最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色

	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	FragColor = vec4(result, 1.0f);

}
