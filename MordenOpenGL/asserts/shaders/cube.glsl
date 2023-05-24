#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;		// 顶点位置
layout (location = 1) in vec3 a_Normal;		// 顶点法线
layout (location = 2) in vec2 a_TexCoords;		// 纹理坐标

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

	// 法线矩阵被定义为「模型矩阵左上角3x3部分的逆矩阵的转置矩阵」
	// 每当我们应用一个不等比缩放时（注意：等比缩放不会破坏法线，因为法线的方向没被改变，仅仅改变了法线的长度，而这很容易通过标准化来修复），法向量就不会再垂直于对应的表面了，这样光照就会被破坏。

	// 修复这个行为的诀窍是使用一个为法向量专门定制的模型矩阵。这个矩阵称之为法线矩阵(Normal Matrix)，它使用了一些线性代数的操作来移除对法向量错误缩放的影响。
	// 在顶点着色器中，我们可以使用inverse和transpose函数自己生成这个法线矩阵，这两个函数对所有类型矩阵都有效。
	// 注意我们还要把被处理过的矩阵强制转换为3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量。
	Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	TexCoords = a_TexCoords;

	gl_Position = u_Projection * u_View * vec4(FragPos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

// 材质属性
//struct Material
//{
//	vec3 ambient;		// 环境光照
//	vec3 diffuse;		// 漫反射光照
//	vec3 specular;		// 镜面光照
//	float shininess;	// 反光度
//};

// 纹理贴图材质属性
struct Material
{
	sampler2D diffuse;		// 漫反射贴图
	sampler2D specular;		// 镜面反射贴图
	float shininess;		// 反光度
};

// 光照属性
struct Light
{
	vec3 position;		// 光源位置

	vec3 ambient;		// 环境光照强度
	vec3 diffuse;		// 漫反射光照强度
	vec3 specular;		// 镜面光照强度
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 u_ViewPos;		// 摄像机位置

uniform Material u_Material;	// 材质
uniform Light u_Light;			// 光照

void main()
{

	// 环境光
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, TexCoords).rgb;

	// 漫反射
	vec3 norm = normalize(Normal);						// 向量归一化
	vec3 lightDir = normalize(u_Light.position - FragPos);	// 计算光源到顶点的方向向量并归一化，（光线入射方向）
	float diff = max(dot(norm, lightDir), 0.0f);		// 用光线方向和顶点法线做点乘，点乘结果就是就是两个射线的夹角（光线和顶点法线），这个夹角就表示了光源对当前顶点的实际漫反射效果
	// 当计算光照时我们通常不关心一个向量的模长或它的位置，我们只关心它们的方向。所以，几乎所有的计算都使用单位向量完成，因为这简化了大部分的计算（比如点乘）。
	// 所以当进行光照计算时，确保你总是对相关向量进行标准化，来保证它们是真正地单位向量。忘记对向量进行标准化是一个十分常见的错误。
	// 同时确保这个点乘值取大于0的值，因为小于0表示的光照是没有意义的
	vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, TexCoords).rgb;		// 这里对于实际贴图渲染的漫反射和环境光的渲染，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以这里使用同一个属性进行计算

	// 镜面光照
	vec3 viewDir = normalize(u_ViewPos - FragPos);	// 视线方向向量
	vec3 reflectDir = reflect(-lightDir, norm);		// 沿着法线轴的反射向量
	// 需要注意的是我们对lightDir向量进行了取反。
	// reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
	// 为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。第二个参数要求是一个法向量，所以我们提供的是已标准化的norm向量。
	// 计算镜面分量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);	// 我们先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)。
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
	vec3 specular = u_Light.specular * spec * texture(u_Material.specular, TexCoords).rgb; // 最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);

	// FragColor = texture(u_Material.specular, TexCoords);

}
