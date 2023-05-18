#include "hzpch.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front_(glm::vec3(0.0f, 0.0f, -1.0f)), camera_speed_(2.5f), sensitivity_(0.1f), zoom_(45.0f)
{
	position_ = position;
	world_up_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors_();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(int direction, float deltaTime)
{
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
}

void Camera::ProcessMouseScroll(float yoffset)
{
}

void Camera::UpdateCameraVectors_()
{
}
