#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
//layout (location = 3) in vec3 a_Tangent;
//layout (location = 4) in vec3 a_BiTangent;
//layout (location = 5) in vec4 a_BoneIDs;
//layout (location = 6) in vec4 a_Weights;

// uniform mat4 u_Transform;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	TexCoords = a_TexCoords;
	Normal = a_Normal;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}

// 片段着色器
#type fragment
#version 330 core

#define NR_POINT_LIGHTS 4	// 点光源数量

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

// 定向光（太阳）
struct DirLight
{
	vec3 direction;		// 光照方向

	vec3 ambient;		// 环境光照强度
	vec3 diffuse;		// 漫反射光照强度
	vec3 specular;		// 镜面光照强度

};

// 点光源
struct PointLight
{
	vec3 position;		// 光源位置

	vec3 ambient;		// 环境光照强度
	vec3 diffuse;		// 漫反射光照强度
	vec3 specular;		// 镜面光照强度

	float constant;		// 常数项
    float linear;		// 一次项
    float quadratic;	// 二次项
};

// 聚光灯
struct SpotLight
{
	vec3 position;		// 光源位置

	vec3 direction;		// 聚光所指向的方向。
	float cutOff;		// 聚光的切光角（内切光）
	float outerCutOff;	// 聚光灯的切光角（外切光）

	vec3 ambient;		// 环境光照强度
	vec3 diffuse;		// 漫反射光照强度
	vec3 specular;		// 镜面光照强度

	float constant;		// 常数项
    float linear;		// 一次项
    float quadratic;	// 二次项
};

uniform vec3 u_ViewPos;		// 摄像机位置

uniform DirLight u_DirLight;	// 定向光照
uniform PointLight u_PointLights[NR_POINT_LIGHTS];	// 点光源数组
uniform SpotLight u_SpotLight;	// 聚光灯

uniform sampler2D u_DiffuseTexture1;
uniform sampler2D u_SpecularTexture1;
//uniform sampler2D u_NormalTexture1;
//uniform sampler2D u_HeightTexture1;

// 光照计算函数定义
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

	// 预处理
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	// 我们的照明被分为3个阶段：方向光、点光源和可选聚光灯
	// 对于每个阶段，都定义了一个计算函数，用于计算相应灯的颜色。
	// 在main()函数中，我们将所有计算出的颜色相加，得到这个片段最终的颜色。

	// 定向光
	vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
	// 点光源
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(u_PointLights[i], norm, FragPos, viewDir);
	}

	// 聚光灯
	result += CalcSpotLight(u_SpotLight, norm, FragPos, viewDir);
	
	FragColor = vec4(result, 1.0);

	// FragColor = texture(u_DiffuseTexture1, TexCoords);
	// FragColor = vec4(TexCoords.x, TexCoords.y, 0.0, 1.0);
	FragColor = vec4(Normal, 1.0);
	// FragColor = texture(u_DiffuseTexture1, TexCoords) + texture(u_SpecularTexture1, TexCoords) + texture(u_NormalTexture1, TexCoords) + texture(u_HeightTexture1, TexCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);

	// 组装结果
	vec3 ambient = light.ambient * vec3(texture(u_DiffuseTexture1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_DiffuseTexture1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(u_SpecularTexture1, TexCoords));

	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	// 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	// 光源和顶点的距离
	float distances = length(light.position - fragPos);
	// 光照强度衰减值
	float attenuation = 1.0 / (light.constant + light.linear * distances + light.quadratic * (distances * distances));
	
	vec3 ambient = light.ambient * vec3(texture(u_DiffuseTexture1, TexCoords));			// 环境光
	vec3 diffuse = light.diffuse * diff * vec3(texture(u_DiffuseTexture1, TexCoords));		// 漫反射
	vec3 specular = light.specular * spec * vec3(texture(u_SpecularTexture1, TexCoords));	// 镜面反射

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);	// 计算光源到顶点的方向向量并归一化，（光线入射方向）
	// 光源和顶点的距离
	float distances = length(light.position - fragPos);
	// 光线衰减值
	float attenuation = 1.0f / (light.constant + light.linear * distances + light.quadratic * (distances * distances));

	// 环境光
    vec3 ambient = light.ambient * texture(u_DiffuseTexture1, TexCoords).rgb;

	// 漫反射
	float diff = max(dot(normal, lightDir), 0.0f);		// 用光线方向和顶点法线做点乘，点乘结果就是就是两个射线的夹角（光线和顶点法线），这个夹角就表示了光源对当前顶点的实际漫反射效果
	// 当计算光照时我们通常不关心一个向量的模长或它的位置，我们只关心它们的方向。所以，几乎所有的计算都使用单位向量完成，因为这简化了大部分的计算（比如点乘）。
	// 所以当进行光照计算时，确保你总是对相关向量进行标准化，来保证它们是真正地单位向量。忘记对向量进行标准化是一个十分常见的错误。
	// 同时确保这个点乘值取大于0的值，因为小于0表示的光照是没有意义的
	vec3 diffuse = light.diffuse * diff * texture(u_DiffuseTexture1, TexCoords).rgb;		// 这里对于实际贴图渲染的漫反射和环境光的渲染，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以这里使用同一个属性进行计算

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);		// 沿着法线轴的反射向量
	// 需要注意的是我们对lightDir向量进行了取反。
	// reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
	// 为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。第二个参数要求是一个法向量，所以我们提供的是已标准化的norm向量。
	// 计算镜面分量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);	// 我们先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)。
	// 一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。
	vec3 specular = light.specular * spec * texture(u_SpecularTexture1, TexCoords).rgb; // 最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色

	// 光线计算加上距离衰减值
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	// LightDir向量和SpotDir向量之间的夹角。在聚光内部的话θ值应该比ϕ值小。
	float theta = dot(lightDir, normalize(-light.direction));
	// 这里ϵ(Epsilon)是内（ϕ）和外圆锥（γ）之间的余弦值差（ϵ=ϕ−γ）。
	float epsilon = light.cutOff - light.outerCutOff;
	// 最终的I值就是在当前片段聚光的强度。
	// 注意我们使用了clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间。这保证强度值不会在[0, 1]区间之外
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	// 执行光照计算
	return (ambient + diffuse + specular);
}
