#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "MordenOpenGL/Shader.h"
#include "MordenOpenGL/Camera.h"
#include "MordenOpenGL/Model.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MordenOpenGL/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static uint32_t SCR_WIDTH = 800;
static uint32_t SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera;
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", nullptr, nullptr);
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

	// glfwSetCursorPosCallback(window, mouse_callbak);
	glfwSetScrollCallback(window, scroll_callback);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 初始化GLAD，传入用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glGetError();

	glEnable(GL_DEPTH_TEST);

	// glEnable(GL_PROGRAM_POINT_SIZE);

	Shader planShader("asserts/shaders/plant.glsl");
	Shader rockShader("asserts/shaders/rock.glsl");

	Model plant("asserts/model/planet/planet.obj", true);
	Model rock("asserts/model/rock/rock.obj", true);


	unsigned int amount = 1000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	std::srand(glfwGetTime());	// 初始化随机种子
	float radius = 50.f;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1。位移：分布在半径为radius的圆形上，偏移范围是[-offset, offset]
		float angle = (float)i / (float)amount * 360.f;
		float displacement = (std::rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (std::rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float y = displacement * 0.4f;		// 让行星带的高度比x和z的宽度要小
		displacement = (std::rand() % (int)(2 * offset * 100)) / 100.f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2.缩放：在0.05和0.25之间缩放
		float scale = (std::rand() % 20) / 100.f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3.旋转：绕着一个（半）随机悬着的旋转轴向量进行随机的旋转
		float rotAngle = (std::rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4.添加到矩阵的数组中
		modelMatrices[i] = model; 
	}

	uint32_t buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// 将变换矩阵设置为实例顶点属性（使用除数 1）
	// 注意：我们在一定程度上作弊，通过获取模型网格的现在公开声明的 VAO，并添加新的 glVertexAttribPointer
	// 通常你会希望以更有组织的方式完成这个过程，但出于学习目的，这样做就足够了。
	for (auto& mesh: rock.GetMeshs())
	{
		uint32_t VAO = mesh.GetVAO();
		glBindVertexArray(VAO);
		// // 为矩阵（4 个 vec4）设置属性指针
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

	}
	

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

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
		glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClear(GL_COLOR_BUFFER_BIT);

		// 配置变换矩阵
		rockShader.Use();
		rockShader.SetMat4("u_Projection", camera.GetProjection());
		rockShader.SetMat4("u_View", camera.GetViewMatrix());

		// 绘制行星
		planShader.Use();
		planShader.SetMat4("u_Projection", camera.GetProjection());
		planShader.SetMat4("u_View", camera.GetViewMatrix());
		glm::mat4 mmodel{ 1.0f };
		mmodel = glm::translate(mmodel, glm::vec3(0.0f, -3.0f, 0.0f));
		mmodel = glm::scale(mmodel, glm::vec3(4.0f, 4.0f, 4.0f));
		planShader.SetMat4("u_Model", mmodel);
		plant.Draw(planShader);

		//// 绘制小行星带
		//rockShader.Use();
		//rockShader.SetInt("u_DiffuseTexture1", 0);
		//glActiveTexture(GL_TEXTURE0);
		//rock.GetLoadedTexture()[0].Bind(GL_TEXTURE_2D);
		//// std::cout << "[1]" << glGetError() << '-' << rock.GetLoadedTexture()[0].GetRendererID() << '-' << std::endl; // 返回 0 (无错误
		//for (auto &mesh : rock.GetMeshs())
		//{
		//	// mesh.Draw(rockShader);
		//	glBindVertexArray(mesh.GetVAO());
		//	glDrawElementsInstanced(GL_TRIANGLES, static_cast<uint32_t>(mesh.GetIndicesSize()), GL_UNSIGNED_INT, 0, amount);
		//	glBindVertexArray(0);
		//}
		
		// 检查并调用事件，交换缓冲
		// glfw：交换缓冲区和轮询 IO 事件（按下/释放键、移动鼠标等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}

// ---------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
	camera.SetViewportSize(static_cast<float>(width), static_cast<float>(height));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else
	{
		// 摄像机
		camera.OnUpdate(window, deltaTime);
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

		// camera.ProcessMouseMovement(xoffset, yoffset);
	}

	
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// camera.ProcessMouseScroll(static_cast<float>(yoffset));
	camera.OnMouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

