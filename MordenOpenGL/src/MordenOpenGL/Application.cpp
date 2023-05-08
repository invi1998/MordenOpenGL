#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

constexpr uint32_t SCR_WIDTH = 800;
constexpr uint32_t SCR_HEIGHT = 600;

// 窗口大小调整时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 退出按钮事件
void processInput(GLFWwindow* window);


// 顶点着色器
const std::string vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 a_Pos;

void main()
{
	gl_Position = vec4(a_Pos, 1.0f);
}
)";

// 片段着色器
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

	// glfw 初始化glfw和配置OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// MAC 下需要添加下面这行代码，配置才能生效
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
	glfwMakeContextCurrent(window);

	// 初始化GLAD，传入用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置视口
	glViewport(0, 0, 800, 600);

	// 注册窗口resize时的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 创建shader对象
	// ---------------------------------------------------------------------
	// 顶点着色器
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// 编译shader
	glShaderSource(vertexShader, 1, (const GLchar**)vertexShaderSource.c_str(), nullptr);
	glCompileShader(vertexShader);

	// 检查shader是否编译成功
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
	// 片段着色器
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

	// 链接着色器
	uint32_t shaderProgram = glCreateProgram();
	glAttachShader()


	// 渲染循环
	while(!glfwWindowShouldClose(window))
	{
		// 事件输入
		processInput(window);

		// 渲染指令
		glClearColor(0.1f, 0.1f, 0.215f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 检查并调用事件，交换缓冲
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
