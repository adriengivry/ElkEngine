#pragma once

#include <ElkAPI/IDataStructure.h>

namespace ElkRendering
{
	namespace Data
	{
		// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
		class CameraData : public ElkAPI::IDataStructure
		{
		private:
			// Default camera values
			const static float PITCH;
			const static float YAW;
			const static float ROLL;
			const static float SPEED;
			const static float SENSITIVITY;
			const static float ZOOM;

			// CameraData Attributes
			glm::vec3 m_position;
			glm::vec3 m_front;
			glm::vec3 m_up;
			glm::vec3 m_right;
			glm::vec3 m_worldUp;

			// Euler Angles
			float m_pitch;
			float m_yaw;
			float m_roll;

			// CameraData options
			float m_movementSpeed;
			float m_mouseSensitivity;
			float m_zoom;

		public:
			// Constructor with vectors
			explicit CameraData(const glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 5.0f),
				const glm::vec3 p_up = glm::vec3(0.0f, 1.0f, 0.0f),
				const float p_pitch = PITCH,
				const float p_yaw = YAW,
				const float p_roll = ROLL);

			// Constructor with scalar values
			CameraData(const float p_posX, const float p_posY, const float p_posZ,
				const float p_upX, const float p_upY, const float p_upZ,
				 const float p_pitch, const float p_yaw, const float p_roll);

			virtual ~CameraData() = default;

			void Setup() override;

			// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
			glm::mat4 GetViewMatrix();

			void ProcessRotationMovement(const glm::vec3 & p_rotation);

			const glm::vec3& GetPosition() const { return m_position; }
			void SetPosition(const glm::vec3& p_position) { m_position = p_position; }
			const glm::vec3& GetFront() const { return m_front; }
			void SetFront(const glm::vec3& p_front) { m_front = p_front; }
			const glm::vec3& GetUp() const { return m_up; }
			void SetUp(const glm::vec3& p_up) { m_up = p_up; }
			const glm::vec3& GetRight() const { return m_right; }
			void SetRight(const glm::vec3& p_right) { m_right = p_right; }
			const glm::vec3& GetWorldUp() const { return m_worldUp; }
			void SetWorldUp(const glm::vec3& p_worldUp) { m_worldUp = p_worldUp; }
			float GetYaw() const { return m_yaw; }
			void SetYaw(const float p_yaw) { m_yaw = p_yaw; }
			float GetPitch() const { return m_pitch; }
			void SetPitch(const float p_pitch) { m_pitch = p_pitch; }
			float GetRoll() const { return m_roll; }
			void SetRoll(const float p_roll) { m_roll = p_roll; }
			float GetMovementSpeed() const { return m_movementSpeed; }
			void SetMovementSpeed(const float p_movementSpeed) { m_movementSpeed = p_movementSpeed; }
			float GetMouseSensitivity() const { return m_mouseSensitivity; }
			void SetMouseSensitivity(const float p_mouseSensitivity) { m_mouseSensitivity = p_mouseSensitivity; }
			float GetZoom() const { return m_zoom; }
			void SetZoom(const float p_zoom) { m_zoom = p_zoom; }

		private:
			// Calculates the front vector from the CameraData's (updated) Euler Angles
			void UpdateCameraVectors();
		public:
		};
	}
}

