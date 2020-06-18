#include "MousePicker.h"

MousePicker::MousePicker(Camera camera, glm::mat4 projection, double xPos, double yPos, int width, int height) 
	:m_Camera(camera), m_Projection(projection), m_MouseX(xPos), m_MouseY(yPos), m_Width(width), m_Height(height)
{
	m_View = camera.UpdateView();
	m_Ray = glm::vec3(0.0f);

	CalculateRay();
}

MousePicker::~MousePicker() {
	//holder
}

void MousePicker::CalculateRay() {
	//Convert 2D viewport space to normalized device coordinates
	float x = (2.0f * m_MouseX) / m_Width - 1.0f;
	float y = 1.0f - (2.0f * m_MouseY) / m_Height; //OpenGL's Y 0 is top, thus why -1.0 is in front

	glm::vec4 clipCoords = glm::vec4(x, y, -1.0f, 1.0f); //Go to homogeneous clip space

	//Convert from clip space to eye space
	glm::vec4 rayEye = glm::inverse(m_Projection) * clipCoords;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	//Eye space to world space
	glm::vec4 worldRay = glm::inverse(m_View) * rayEye;
	m_Ray = glm::normalize(glm::vec3(worldRay.x, worldRay.y, worldRay.z));
}
