#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp, float sensitivty)
	:m_Pos(position), m_Front(target), m_Up(worldUp), m_CameraSens(sensitivty)
{
	m_FirstMouse = true;
	m_FreemodeActive = false;
	m_JumpActive = false;

	m_Yaw = -90.0f;
	m_Pitch = 0.0f;
	m_LastX = 0;
	m_LastY = 0;

	m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}

Camera::~Camera() {
	//holder
}

glm::mat4 Camera::UpdateView() {
	return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
}

void Camera::Move(Direction dir, float deltaTime) {
	float moveSpeed = 3.5f * deltaTime;
	float wantedY = 0.0f;

	switch (dir) {
		case Direction::FOWARD:
			m_Pos += (m_FreemodeActive) ? (m_Front * moveSpeed) : glm::normalize(glm::cross(m_Up, m_Right)) * moveSpeed; //If not in freemode move to where the front vector is (not camera front)
			break;

		case Direction::BACK:
			m_Pos -= (m_FreemodeActive) ? (m_Front * moveSpeed) : glm::normalize(glm::cross(m_Up, m_Right)) * moveSpeed;
			break;

		case Direction::LEFT:
			m_Pos -= m_Right * moveSpeed;
			break;
		
		case Direction::RIGHT:
			m_Pos += m_Right * moveSpeed;
			break;

		default:
			break;
	}
}

void Camera::MouseMove(double xPos, double yPos) {
	if (m_FirstMouse) {
		m_LastX = xPos;
		m_LastY = yPos;
		m_FirstMouse = false;
	}

	float xOffset = xPos - m_LastX;
	float yOffset = m_LastY - yPos;
	m_LastX = xPos;
	m_LastY = yPos;

	xPos *= m_CameraSens;
	yPos *= m_CameraSens;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (m_Pitch > 89.0f) {
		m_Pitch = 89.0f;
	}
	else if (m_Pitch < -89.0f) {
		m_Pitch = -89.0f;
	}

	if (m_Yaw > 360.0f || m_Yaw < -360.0f) {
		m_Yaw = 0.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_Up)); //Where the right of your camera is always changing so you have to keep updating it
}

void Camera::Fall(float wantedY) {
	m_Pos.y = wantedY;
	m_JumpActive = false;
}

void Camera::Jump() {
	if (!m_JumpActive) {
		m_Pos.y += 1;
		m_JumpActive = true;
	}
}

void Camera::SetSensitivty(float newSens) {
	m_CameraSens = newSens;
}

void Camera::SetFreemode() {
	m_FreemodeActive = !m_FreemodeActive;
}

void Camera::SetPos(glm::vec3 newPos) {
	m_Pos = newPos;
}

void Camera::SetJumpActive(bool active) {
	m_JumpActive = false;
}
