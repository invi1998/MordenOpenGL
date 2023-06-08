#type vertex
#version 330 core

layout (location = 0) in vec2 a_Pos;

void main()
{
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0f, 1.0f);
}

// 几何着色器
#type geometry
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

void build_house(vec4 position)
{    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
    EmitVertex();
    EndPrimitive();
}

void main()
{
	build_house(gl_in[0].gl_Position);
}


// 片段着色器
#type fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
