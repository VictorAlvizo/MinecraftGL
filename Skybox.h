#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <SOIL2.h>
#include "VBO.h"

class Skybox {
public:
	Skybox(std::vector<std::string> imagePaths);
	~Skybox();

	void Draw();

private:
	void SetTextures(std::vector<std::string>& imagePaths);

	VBO * m_VBO;

	unsigned int m_Texture;
	unsigned char * data;

	int m_Width, m_Height, m_Channals;
};