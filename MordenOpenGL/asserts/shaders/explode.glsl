#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	vs_out.TexCoords = a_TexCoords;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

// 几何着色器
#type geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
} gs_in[];

uniform float time;

out vec2 TexCoords;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 200.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 1.0);
}

vec3 GetNormal()
{
    // 因为我们想要沿着三角形的法向量位移每个顶点，所以我们需要首先计算出这个法向量。
    // 我们要做的就是计算垂直与三角形表面的向量，仅使用我们能够访问的三个顶点。
    // 我们使用叉乘来获取垂直与其他两个向量的一个向量，我们能够获取两个平行与三角形表面的向量a和b，我们就能对这两个向量进行叉乘来获取法向量了
    // 其实说白了，a和b就是这个三角形的两个边，然后使用叉乘就能计算出垂直于这两条边组成的平面的一个向量，就是我们要的三角形面法向量了
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main()
{

    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].TexCoords;
    EmitVertex();
    EndPrimitive();
}


// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}
