#include "stdafxRendering.h"

#include "ElkRendering/Data/CameraData.h"

using namespace ElkRendering::Data;

// Default camera values
const float CameraData::PITCH = 0.0f;
const float CameraData::YAW = -90.0f;
const float CameraData::ROLL = 0.0f;
const float CameraData::SPEED = 2.5f;
const float CameraData::SENSITIVITY = 0.1f;
const float CameraData::ZOOM = 45.0f;

CameraData::CameraData(
	const glm::vec3 p_position,
	const glm::vec3 p_up,
	const float p_pitch, 
	const float p_yaw,
	const float p_roll)
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	  m_movementSpeed(SPEED),
	  m_mouseSensitivity(SENSITIVITY),
	  m_zoom(ZOOM)
{
	m_position = p_position;
	m_worldUp = p_up;

	m_pitch = p_pitch;
	m_yaw = p_yaw;
	m_roll = p_roll;
	UpdateCameraVectors();
}

CameraData::CameraData(
	const float p_posX, const float p_posY, const float p_posZ,
	const float p_upX, const float p_upY, const float p_upZ,
	 const float p_pitch, const float p_yaw, const float p_roll)
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	  m_movementSpeed(SPEED),
	  m_mouseSensitivity(SENSITIVITY),
	  m_zoom(ZOOM)
{
	m_position = glm::vec3(p_posX, p_posY, p_posZ);
	m_worldUp = glm::vec3(p_upX, p_upY, p_upZ);

	m_pitch = p_pitch;
	m_yaw = p_yaw;
	m_roll = p_roll;
	UpdateCameraVectors();
}

void CameraData::Setup()
{
}

glm::mat4 CameraData::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void CameraData::ProcessRotationMovement(const glm::vec3& p_rotation)
{
	m_pitch = p_rotation.x;
	m_yaw = p_rotation.y;
	m_roll = p_rotation.z;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	// Update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void CameraData::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));

	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));

	// calculate the right vector
	m_right = glm::normalize(
		m_right * cos(m_roll) + m_up * sin(m_roll)
	);

	// calculate the new up vector
	m_up = glm::cross(m_front, m_right);
	m_up *= -1;
}
