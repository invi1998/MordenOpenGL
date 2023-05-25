#include "hzpch.h"

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include "MordenOpenGL/Shader.h"
#include "MordenOpenGL/Camera.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MordenOpenGL/Texture.h"

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

	glEnable(GL_DEPTH_TEST);

	// �����ӿ�
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// ����������shader�ļ�
	Shader lightShader("asserts/shaders/light.glsl");
	Shader cubeShader("asserts/shaders/cube.glsl");

	// ��������
	float vertices[] = {
		// ---- λ�� ---      - ���㷨��������Ϊ����������Ķ������ķ������Ƚϼ򵥣���������ֱ�Ӵ��룩 -       - �������� -
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

	// ���Դλ��
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// ���Դ��ɫ
	glm::vec3 pointLightColor[] = {
		glm::vec3(0.7f,  0.2f,  1.0f),
		glm::vec3(0.3f, 0.3f, 0.8f),
		glm::vec3(0.9f,  0.3f, 0.125f),
		glm::vec3(0.2f,  0.9f, 0.32f)
	};

	uint32_t VBO, cubeVAO;

	// ��������������󣬶��㻺�����Ԫ�ػ������
	glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);

	// �󶨶������ݶ���Ȼ��󶨲������û��壬Ȼ�����ö�������
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// ������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ��Դ������
	uint32_t lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// ��������
	Texture diffuseMap("asserts/textures/container2.png");		// ��������ͼ
	Texture specularMap("asserts/textures/container2_specular.png");		// �߹⣨����⣩��ͼ

	cubeShader.Use();
	cubeShader.SetInt("u_Material.diffuse", 0);
	cubeShader.SetInt("u_Material.specular", 1);

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

		// ��Դ
		glm::vec3 lightColor{ 1.0f };
		// lightColor.x = static_cast<float>(sin(glfwGetTime() + 2.4f));
		// lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.8f));
		// lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.5f));

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 3.0f;

		cubeShader.Use();
		// ��������
		cubeShader.SetVec3("u_Light.position", lightPos);
		cubeShader.SetVec3("u_Light.direction", camera.GetForwardDirection());
		cubeShader.SetFloat("u_Light.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.SetFloat("u_Light.outerCutOff", glm::cos(glm::radians(17.5f)));
		cubeShader.SetVec3("u_ViewPos", camera.GetPosition());
		cubeShader.SetVec3("u_Light.ambient", ambientColor);
		cubeShader.SetVec3("u_Light.diffuse", diffuseColor);
		cubeShader.SetVec3("u_Light.specular", glm::vec3{1.0f, 1.0f, 1.0f});
		cubeShader.SetFloat("u_Light.constant", 1.0f);
		cubeShader.SetFloat("u_Light.linear", 0.09f);
		cubeShader.SetFloat("u_Light.quadratic", 0.032f);


		// directional light
		cubeShader.SetVec3("u_DirLight.direction", lightPos);
		cubeShader.SetVec3("u_DirLight.ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		cubeShader.SetVec3("u_DirLight.diffuse", glm::vec3{ 0.4f, 0.4f, 0.4f });
		cubeShader.SetVec3("u_DirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});
		// point light 1
		cubeShader.SetVec3("u_PointLights[0].position", pointLightPositions[0]);
		cubeShader.SetVec3("u_PointLights[0].lightColor", pointLightColor[0]);
		cubeShader.SetVec3("u_PointLights[0].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		cubeShader.SetVec3("u_PointLights[0].diffuse",glm::vec3{ 0.8f, 0.8f, 0.8f });
		cubeShader.SetVec3("u_PointLights[0].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetFloat("u_PointLights[0].constant", 1.0f);
		cubeShader.SetFloat("u_PointLights[0].linear", 0.09f);
		cubeShader.SetFloat("u_PointLights[0].quadratic", 0.032f);
		// point light 2
		cubeShader.SetVec3("u_PointLights[1].position", pointLightPositions[1]);
		cubeShader.SetVec3("u_PointLights[1].lightColor", pointLightColor[1]);
		cubeShader.SetVec3("u_PointLights[1].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		cubeShader.SetVec3("u_PointLights[1].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		cubeShader.SetVec3("u_PointLights[1].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetFloat("u_PointLights[1].constant", 1.0f);
		cubeShader.SetFloat("u_PointLights[1].linear", 0.09f);
		cubeShader.SetFloat("u_PointLights[1].quadratic", 0.032f);
		// point light 3
		cubeShader.SetVec3("u_PointLights[2].position", pointLightPositions[2]);
		cubeShader.SetVec3("u_PointLights[2].lightColor", pointLightColor[2]);
		cubeShader.SetVec3("u_PointLights[2].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		cubeShader.SetVec3("u_PointLights[2].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		cubeShader.SetVec3("u_PointLights[2].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetFloat("u_PointLights[2].constant", 1.0f);
		cubeShader.SetFloat("u_PointLights[2].linear", 0.09f);
		cubeShader.SetFloat("u_PointLights[2].quadratic", 0.032f);
		// point light 4
		cubeShader.SetVec3("u_PointLights[3].position", pointLightPositions[3]);
		cubeShader.SetVec3("u_PointLights[3].lightColor", pointLightColor[3]);
		cubeShader.SetVec3("u_PointLights[3].ambient", glm::vec3{ 0.05f, 0.05f, 0.05f });
		cubeShader.SetVec3("u_PointLights[3].diffuse", glm::vec3{ 0.8f, 0.8f, 0.8f });
		cubeShader.SetVec3("u_PointLights[3].specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetFloat("u_PointLights[3].constant", 1.0f);
		cubeShader.SetFloat("u_PointLights[3].linear", 0.09f);
		cubeShader.SetFloat("u_PointLights[3].quadratic", 0.032f);
		// spotLight
		cubeShader.SetVec3("u_SpotLight.position", camera.GetPosition());
		cubeShader.SetVec3("u_SpotLight.direction", camera.GetForwardDirection());
		cubeShader.SetVec3("u_SpotLight.ambient", ambientColor);
		cubeShader.SetVec3("u_SpotLight.diffuse", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetVec3("u_SpotLight.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
		cubeShader.SetFloat("u_SpotLight.constant", 1.0f);
		cubeShader.SetFloat("u_SpotLight.linear", 0.09f);
		cubeShader.SetFloat("u_SpotLight.quadratic", 0.032f);
		cubeShader.SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// ��������
		// cubeShader.SetVec3("u_Material.ambient", glm::vec3{ 0.8, 0.64, 0.21 });
		// cubeShader.SetVec3("u_Material.diffuse", glm::vec3{ 0.8, 0.64, 0.21 });
		// cubeShader.SetVec3("u_Material.specular", glm::vec3{ 0.5, 0.5, 0.5 });
		cubeShader.SetFloat("u_Material.shininess", 64.0f);
		// ��ͼ��͸��ͶӰ
		glm::mat4 projection = camera.GetProjection();
		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.SetMat4("u_Projection", projection);
		cubeShader.SetMat4("u_View", view);
		// ����任
		glm::mat4 model{ 1.0f };
		

		glEnable(GL_BLEND);

		// �����������
		glActiveTexture(GL_TEXTURE0);
		diffuseMap.Bind(GL_TEXTURE_2D);
		// �󶨾��淴�����
		glActiveTexture(GL_TEXTURE1);
		specularMap.Bind(GL_TEXTURE_2D);

		// ��Ⱦ������
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.SetMat4("u_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightShader.Use();
		lightShader.SetVec3("u_LightColor", lightColor);
		lightShader.SetMat4("u_Projection", projection);
		lightShader.SetMat4("u_View", view);

		glBindVertexArray(lightVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
		lightShader.SetMat4("u_Model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ��Ⱦ������
		glBindVertexArray(lightVAO);
		for (uint32_t i = 0; i < 4; i++)
		{
			// ��������任
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader.SetMat4("u_Model", model);
			lightShader.SetVec3("u_LightColor", pointLightColor[i]);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// ��鲢�����¼�����������
		// glfw����������������ѯ IO �¼�������/�ͷż����ƶ����ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ��ѡ������Դ������;��ȡ������������Դ��
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

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
	camera.OnMouseScroll(xoffset, yoffset);
}
