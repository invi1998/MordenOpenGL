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

// ���ڴ�С����ʱ�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// �˳���ť�¼�
void processInput(GLFWwindow* window);

// ����¼�
void mouse_callbak(GLFWwindow* window, double xposIn, double yposIn);

// �������¼�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳����������ˡ�
	glfwMakeContextCurrent(window);

	// ע�ᴰ��resizeʱ�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glfwSetCursorPosCallback(window, mouse_callbak);
	glfwSetScrollCallback(window, scroll_callback);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ��ʼ��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glGetError();

	glEnable(GL_DEPTH_TEST);

	// glEnable(GL_PROGRAM_POINT_SIZE);

	Shader instanceShader("asserts/shaders/instancing.glsl");

	float quadVertices[] = {
		// λ��          // ��ɫ
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};

	glm::vec2 translations[100];
	int index = 0;
	const float offset = 0.1f;
	for (int y = -10; y < 10; y+= 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = static_cast<float>(x) / 10.f + offset;
			translation.y = static_cast<float>(y) / 10.f + offset;
			translations[index++] = translation;
		}
	}

	uint32_t instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint32_t quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	// ����ʵ������
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// ��Ⱦѭ��
	while(!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// �¼�����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClear(GL_COLOR_BUFFER_BIT);

		instanceShader.Use();
		/*for (unsigned int i = 0; i < 100; i++)
		{
			std::stringstream ss;
			std::string index;
			ss << i;
			index = ss.str();
			instanceShader.SetVec2(("u_Offsets[" + index + "]").c_str(), translations[i]);
		}*/

		glBindVertexArray(quadVAO);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

		glBindVertexArray(0);
		
		// ��鲢�����¼�����������
		// glfw����������������ѯ IO �¼�������/�ͷż����ƶ����ȣ�
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
		// �����
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

