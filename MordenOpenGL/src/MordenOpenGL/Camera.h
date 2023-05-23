#pragma once

#include <glm/glm.hpp>

#include "GLFW/glfw3.h"
#include "glm/gtx/quaternion.hpp"

// 定义了多种摄像机移动的可能选项。用作抽象来避免与特定于窗口系统的输入方法产生关联
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    Camera() = default;
    Camera(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(GLFWwindow* window, float ts);

    float GetDistance() const { return m_Distance; }
    void SetDistance(float distance) { m_Distance = distance; }
    void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjection() const { return m_Projection; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;

    const glm::vec3& GetPosition() const { return m_Position; }
    glm::quat GetOrientation() const;

    float GetPith() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }

    void OnMouseScroll(float xOffset, float yOffset);

private:
    void UpdateProjection();
    void UpdateView();

    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;
	

private:
    float m_FOV = 45.0f;
    float m_AspectRatio = 1.3333f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_Projection{ 1.0f };
    glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_FocalPoint{ 0.0f, 0.0f, 0.0f };

    glm::vec2 m_InitialMousePosition{ 0.0f, 0.0f };

    float m_Distance = 10.0f;
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;

    float m_ViewportWidth = 800.0f;
    float m_ViewportHeight = 600.0f;


};

