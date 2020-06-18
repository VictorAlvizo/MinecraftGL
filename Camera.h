#pragma once
#include <GL/glew.h>
#include <iostream>
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"
#include "Vendor/glm/gtx/string_cast.hpp"
#include "Terrain.h"

enum class Direction {
	FOWARD, BACK, LEFT, RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp, float sensitivty);
	~Camera();

	glm::mat4 UpdateView();

	void Move(Direction dir, float deltaTime);
	void MouseMove(double xPos, double yPos);

	void Fall(float wantedY);
	void Jump();

	void SetSensitivty(float newSens);
	void SetFreemode();

	void SetPos(glm::vec3 newPos);
	void SetJumpActive(bool active);

	inline glm::vec3 getPos() const { return m_Pos; }
	inline glm::vec3 getUp() const { return m_Up; }
	inline glm::vec3 getRight() const { return m_Right; }

	inline bool freemodeActive() const { return m_FreemodeActive; }

private:
	glm::vec3 m_Pos, m_Front, m_Up, m_Right;

	float m_Yaw, m_Pitch;
	float m_CameraSens;
	double m_LastX, m_LastY;

	bool m_FirstMouse;
	bool m_FreemodeActive;
	bool m_JumpActive;
};

