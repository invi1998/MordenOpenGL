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

	Shader modelShader("asserts/shaders/model.glsl");
	Shader lightShader("asserts/shaders/light.glsl");

	// 模型
	Model myModel("asserts/model/nanosuit/nanosuit.obj", true);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// 点光源颜色
	glm::vec3 pointLightColor[] = {
		glm::vec3(0.7f,  0.2f,  1.0f),
		glm::vec3(0.3f, 0.3f, 0.8f),
		glm::vec3(0.9f,  0.3f, 0.125f),
		glm::vec3(0.2f,  0.9f, 0.32f)
	};

	unsigned int VBO;
	
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 设置视口
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

		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 3.0f;

		glm::vec3 lightColor(1.0f);

		modelShader.Use();

		// view/projection transformations
		modelShader.SetMat4("u_Projection", camera.GetProjection());
		modelShader.SetMat4("u_View", camera.GetViewMatrix());

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
		modelShader.SetMat4("u_Model", model);

		modelShader.SetFloat("u_Light.constant", 1.0f);
		modelShader.SetFloat("u_Light.linear", 0.09f);
		modelShader.SetFloat("u_Light.quadratic", 0.032f);

		modelShader.SetVec3("u_ViewPos", camera.GetPosition());


		// directional light
		modelShader.SetVec3("u_DirLight.direction", lightPos);
		modelShader.SetVec3("u_DirLight.ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_DirLight.diffuse", glm::vec3{ 0.4f, 0.4f, 0.4f });
		modelShader.SetVec3("u_DirLight.specular", glm::vec3{ 0.5f, 0.5f, 0.5f });
		// point light 1
		modelShader.SetVec3("u_PointLights[0].position", pointLightPositions[0]);
		modelShader.SetVec3("u_PointLights[0].lightColor", pointLightColor[0]);
		modelShader.SetVec3("u_PointLights[0].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_PointLights[0].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		modelShader.SetVec3("u_PointLights[0].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("u_PointLights[0].constant", 1.0f);
		modelShader.SetFloat("u_PointLights[0].linear", 0.09f);
		modelShader.SetFloat("u_PointLights[0].quadratic", 0.032f);
		// point light 2
		modelShader.SetVec3("u_PointLights[1].position", pointLightPositions[1]);
		modelShader.SetVec3("u_PointLights[1].lightColor", pointLightColor[1]);
		modelShader.SetVec3("u_PointLights[1].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_PointLights[1].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		modelShader.SetVec3("u_PointLights[1].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("u_PointLights[1].constant", 1.0f);
		modelShader.SetFloat("u_PointLights[1].linear", 0.09f);
		modelShader.SetFloat("u_PointLights[1].quadratic", 0.032f);
		// point light 3
		modelShader.SetVec3("u_PointLights[2].position", pointLightPositions[2]);
		modelShader.SetVec3("u_PointLights[2].lightColor", pointLightColor[2]);
		modelShader.SetVec3("u_PointLights[2].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_PointLights[2].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		modelShader.SetVec3("u_PointLights[2].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("u_PointLights[2].constant", 1.0f);
		modelShader.SetFloat("u_PointLights[2].linear", 0.09f);
		modelShader.SetFloat("u_PointLights[2].quadratic", 0.032f);
		// point light 4
		modelShader.SetVec3("u_PointLights[3].position", pointLightPositions[3]);
		modelShader.SetVec3("u_PointLights[3].lightColor", pointLightColor[3]);
		modelShader.SetVec3("u_PointLights[3].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_PointLights[3].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		modelShader.SetVec3("u_PointLights[3].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("u_PointLights[3].constant", 1.0f);
		modelShader.SetFloat("u_PointLights[3].linear", 0.09f);
		modelShader.SetFloat("u_PointLights[3].quadratic", 0.032f);
		// spotLight
		modelShader.SetVec3("u_SpotLight.position", camera.GetPosition());
		modelShader.SetVec3("u_SpotLight.direction", camera.GetForwardDirection());
		modelShader.SetVec3("u_SpotLight.ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		modelShader.SetVec3("u_SpotLight.diffuse", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetVec3("u_SpotLight.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		modelShader.SetFloat("u_SpotLight.constant", 1.0f);
		modelShader.SetFloat("u_SpotLight.linear", 0.09f);
		modelShader.SetFloat("u_SpotLight.quadratic", 0.032f);
		modelShader.SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
		modelShader.SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		myModel.Draw(modelShader);

		lightShader.Use();

		// 材质属性
		lightShader.SetVec3("u_LightColor", lightColor);
		lightShader.SetMat4("u_Projection", camera.GetProjection());
		lightShader.SetMat4("u_View", camera.GetViewMatrix());
		// 世界变换
		model = glm::mat4{ 1.0f };

		glBindVertexArray(lightCubeVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3{ 0.2f });
		model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
		lightShader.SetMat4("u_Model", model);
		lightShader.SetVec3("u_LightColor", lightColor);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 渲染立方体
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		for (uint32_t i = 0; i < 4; i++)
		{
			// 世界坐标变换
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader.SetMat4("u_Model", model);
			lightShader.SetVec3("u_LightColor", pointLightColor[i]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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
	camera.SetViewportSize(width, height);
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
	camera.OnMouseScroll(xoffset, yoffset);
}
