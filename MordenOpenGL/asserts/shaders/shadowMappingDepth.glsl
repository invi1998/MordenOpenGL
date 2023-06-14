#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

// uniform mat4 u_Transform;
uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Pos, 1.0f);
}

// Æ¬¶Î×ÅÉ«Æ÷
#type fragment
#version 330 core


void main()
{
	// gl_FragDepth = gl_FragCoord.z;
}
