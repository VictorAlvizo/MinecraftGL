#pragma once
#include "Camera.h"

class MousePicker
{
public:
	MousePicker(Camera camera, glm::mat4 projection, double xPos, double yPos, int width, int height);
	~MousePicker();

	inline glm::vec3 getRay() const { return m_Ray; }

private:
	void CalculateRay();

	Camera m_Camera;

	glm::mat4 m_Projection;
	glm::mat4 m_View;

	glm::vec3 m_Ray;

	double m_MouseX, m_MouseY;
	int m_Width, m_Height;
};

