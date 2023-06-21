#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_Model;

void main()
{
	// 几何着色器负责将所有世界空间的顶点变换到6个不同的光空间的着色器。
	// 因此顶点着色器简单的将顶点变换到世界空间，然后发给几何着色器
	gl_Position = u_Model * vec4(a_Pos, 1.0f);
}

// 几何着色器
#type geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;	// 从几何着色器中获取的 FragPos（每个 EmitVertex 输出）

// 几何着色器相对简单。我们输入一个三角形，输出总共6个三角形（6*3顶点，所以总共18个顶点）。
// 在main函数中，我们遍历立方体贴图的6个面，我们每个面指定为一个输出面，把这个面的interger（整数）存到gl_Layer。
// 然后，我们通过把面的光空间变换矩阵乘以FragPos，将每个世界空间顶点变换到相关的光空间，生成每个三角形。
// 注意，我们还要将最后的FragPos变量发送给像素着色器，我们需要计算一个深度值。
void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;	// 内置变量，指定我们要渲染到哪个面。
		for (int i = 0;  i < 3;  ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}


// 片段着色器
#type fragment
#version 330 core

in vec4 FragPos;

uniform vec3 u_LightPos;;

uniform float farPlane;

// 我们将计算自己的深度，这个深度就是每个fragment位置和光源位置之间的线性距离。计算自己的深度值使得之后的阴影计算更加直观

void main()
{
	// 获取片段到光源的距离
	float lightDistance = length(FragPos.xyz - u_LightPos);

	// 将距离映射到 [0;1] 范围内，通过除以远平面距离实现
	lightDistance = lightDistance / farPlane;

	// 将计算出的距离作为修改后的深度值进行写入
	gl_FragDepth = lightDistance;
}
