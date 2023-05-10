#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "Shader.h"

#include <stb_image.h>

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

// ���ڴ�С����ʱ�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// �˳���ť�¼�
void processInput(GLFWwindow* window);


// ������ɫ��
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 a_Pos;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);
	vertexColor = a_Pos;
}
)";

// Ƭ����ɫ��
const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec3 vertexColor;

void main()
{
	FragColor = vec4(vertexColor * 0.9 + 0.6, 1.0f);
}
)";

int main(void)
{

	// glfw ��ʼ��glfw������OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// MAC ����Ҫ����������д��룬���ò�����Ч
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// ����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ�
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳����������ˡ�
	glfwMakeContextCurrent(window);

	// ��ʼ��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// �����ӿ�
	glViewport(0, 0, 800, 600);

	// ע�ᴰ��resizeʱ�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ����������shader�ļ�
	Shader outShader("asserts/shaders/test.glsl");

	outShader.SetInt("u_Texture", 0);

	// װ���������ݣ��������һ�������Σ������ö�������
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f   // top left
	};

	// ������Ǵ�0��ʼ
	uint32_t indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	uint32_t VBO, VAO, EBO;

	// ��������������󣬶��㻺�����Ԫ�ػ������
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// �󶨶������ݶ���Ȼ��󶨲������û��壬Ȼ�����ö�������
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// Ӧ����ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Ӧ������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ��ע�⣬��������ģ���glVertexAttribPointer �ĵ��ý�VBOע��Ϊ�������Եİ󶨶��㻺���������Ա�֮�����ǿ��԰�ȫ��ȡ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ���ס���� VAO ���ڻ״̬ʱ����Ҫȡ���� EBO����Ϊ�󶨵�Ԫ�ػ���������洢�� VAO ��;����EBO�󶨡�
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ֮�����ȡ���� VAO���Ա����� VAO ���ò��������޸Ĵ� VAO��������������ٷ�����
	// �޸����� VAO ������ζ���Ҫ����glBindVertexArray����˵�����ֱ����Ҫʱ������ͨ������ȡ����VAOs��Ҳ����VBO����

	glBindVertexArray(0);

	// ȡ����ע�ͣ����ɻ����߿�
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ����
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Ϊ��ǰ���������û��ƣ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���ز���������
	int width, height, nrChannels;
	unsigned char* data = stbi_load("asserts/textures/R-C.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "������ش���" << std::endl;
	}

	stbi_image_free(data);

	// ��Ⱦѭ��
	while(!glfwWindowShouldClose(window))
	{
		// �¼�����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.1f, 0.1f, 0.215f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ����������

		// glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(outShader.GetRendererID());

		// ��������ֻ��һ��VAO�����û�б�Ҫÿ�ζ������������ǻ���������ʹ�����������
		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		// glBindVertexArray(0);	// ����Ҫÿ�ζ����

		// ��鲢�����¼�����������
		// glfw����������������ѯ IO �¼�������/�ͷż����ƶ����ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ��ѡ������Դ������;��ȡ������������Դ��
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(outShader.GetRendererID());

	glfwTerminate();
	return 0;

}

// ---------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
