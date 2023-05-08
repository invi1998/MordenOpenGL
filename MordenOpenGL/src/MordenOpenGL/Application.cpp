#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

// ���ڴ�С����ʱ�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// �˳���ť�¼�
void processInput(GLFWwindow* window);


// ������ɫ��
const std::string vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 a_Pos;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);
}
)";

// Ƭ����ɫ��
const std::string fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.94f, 0.13f, 0.212f, 1.0f);
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

	// ����shader����
	// ---------------------------------------------------------------------
	// ������ɫ��
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// ����shader
	glShaderSource(vertexShader, 1, (const GLchar**)vertexShaderSource.c_str(), nullptr);
	glCompileShader(vertexShader);

	// ���shader�Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// ---------------------------------------------------------------------
	// Ƭ����ɫ��
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)fragmentShaderSource.c_str(), nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_FRAGMENT_SHADER, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// ----------------------------------------------------------------------

	// ������ɫ��
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader()


	// ��Ⱦѭ��
	while(!glfwWindowShouldClose(window))
	{
		// �¼�����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.1f, 0.1f, 0.215f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ��鲢�����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
