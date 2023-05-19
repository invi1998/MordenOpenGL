#include "hzpch.h"
#include "Camera.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = camera_speed_ * deltaTime;

	switch (direction)
	{
	case FORWARD:
		position_ += front_ * velocity;
		break;
	case BACKWARD:
		position_ -= front_ * velocity;
		break;
	case LEFT:
		position_ -= right_ * velocity;
		break;
	case RIGHT:
		position_ += right_ * velocity;
		break;
	default:
		break;

	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= sensitivity_;
	yoffset *= sensitivity_;

	yaw_ += xoffset;
	pitch_ += yoffset;

	if (constrainPitch)
	{
		if (pitch_ > 89.0f)
		{
			pitch_ = 89.0f;
		}
		if (pitch_ < -89.0f)
		{
			pitch_ = -89.0f;
		}
	}
	UpdateCameraVectors_();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom_ >= 1.0f && zoom_ <= 45.0f)
		zoom_ -= yoffset;
	if (zoom_ <= 1.0f)
		zoom_ = 1.0f;
	if (zoom_ >= 45.0f)
		zoom_ = 45.0f;
}

void Camera::UpdateCameraVectors_()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);
	// also re-calculate the Right and Up vector
	right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up_ = glm::normalize(glm::cross(right_, front_));
}