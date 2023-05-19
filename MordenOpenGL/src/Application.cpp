#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "MordenOpenGL/Shader.h"
#include "MordenOpenGL/Camera.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static uint32_t SCR_WIDTH = 800;
static uint32_t SCR_HEIGHT = 600;

static glm::vec3 cameraPos{ 0.0f, 0.0f, 3.0f };
static glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
static glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;

Camera camera(glm::vec3{ 0.0f, 0.0f, 300.0f });

// 窗口大小调整时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 退出按钮事件
void processInput(GLFWwindow* window);

// 鼠标事件
void mouse_callbak(GLFWwindow* window, double xposIn, double yposIn);

// 鼠标滚轮事件
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

	// 注册窗口resize时的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callbak);
	glfwSetScrollCallback(window, scroll_callback);

	// 初始化GLAD，传入用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// 设置视口
	glViewport(0, 0, 800, 600);

	// 这里用外置shader文件
	Shader outShader("asserts/shaders/test.glsl");

	// 装备顶点数据（这里绘制一个三角形），配置顶点属性
	float vertices[] = {
		// ---- 位置 ---      - 纹理坐标 -
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	uint32_t VBO, VAO;

	// 创建顶点数组对象，顶点缓冲对象，元素缓冲对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);

	// 绑定顶点数据对象，然后绑定并且设置缓冲，然后配置顶点属性
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// 应用颜色
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);

	// 应用纹理
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 请注意，这是允许的，对glVertexAttribPointer 的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，以便之后我们可以安全地取消绑定
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 请记住：当 VAO 处于活动状态时，不要取消绑定 EBO，因为绑定的元素缓冲区对象存储在 VAO 中;保持EBO绑定。
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 之后可以取消绑定 VAO，以便其他 VAO 调用不会意外修改此 VAO，但这种情况很少发生。
	// 修改其他 VAO 无论如何都需要调用glBindVertexArray，因此当不是直接需要时，我们通常不会取消绑定VAOs（也不是VBO）。

	// glBindVertexArray(0);

	// 取消该注释，即可绘制线框
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 纹理1
	uint32_t texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 为当前绑定纹理设置环绕，过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int width, height, nrChannels;
	// OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("asserts/textures/R-C.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "纹理加载错误" << std::endl;
	}

	stbi_image_free(data);

	uint32_t texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// 为当前绑定纹理设置环绕，过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("asserts/textures/nofind.png", &width2, &height2, &nrChannels2, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "纹理2加载错误" << std::endl;
	}

	stbi_image_free(data2);

	// 在设置uniform之前，必须先激活shader
	outShader.Use();
	outShader.SetInt("u_Texture1", 0);
	outShader.SetInt("u_Texture2", 1);


	// 渲染循环
	while(!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 事件输入
		processInput(window);

		// 渲染指令
		glClearColor(0.1f, 0.1f, 0.215f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 绘制三角形

		// glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		outShader.Use();

		// 观察矩阵
		glm::mat4 view{ 1.0f };

		/*const float radius = 10.0f;
		float camX = static_cast<float>(sin(glfwGetTime() * radius));
		float camZ = static_cast<float>(cos(glfwGetTime() * radius));

		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/

		// view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		view = camera.GetViewMatrix();

		outShader.SetMat4("u_View", view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 1000.0f);
		outShader.SetMat4("u_Projection", projection);

		glBindVertexArray(VAO);
		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model{ 1.0f };
			model = glm::translate(model, cubePositions[i]);
			float angle = 2000.0f * i;
			// model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			outShader.SetMat4("u_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// trans = glm::rotate(trans, static_cast<float>(0.1 * sin(glfwGetTime())), glm::vec3(0.0f, 0.0f, 1.0f));

		// 然后做 mvp 变化(注意矩阵运算的顺序是相反的（记住我们需要从右往左阅读矩阵的乘法）
		// glm::mat4 trans = projection * view * model;
		// outShader.SetMat4("u_Transform", trans);


		// 鉴于我们只有一个VAO，因此没有必要每次都绑定它，但我们会这样做以使事情更有条理。
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		// glBindVertexArray(0);	// 不需要每次都解绑

		// 检查并调用事件，交换缓冲
		// glfw：交换缓冲区和轮询 IO 事件（按下/释放键、移动鼠标等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 可选：在资源超出用途后取消分配所有资源：
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);
	glDeleteProgram(outShader.GetRendererID());

	glfwTerminate();
	return 0;

}

// ---------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void mouse_callbak(GLFWwindow* window, double xposIn, double yposIn)
{
	static bool is_mouse_pressed = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
