#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SOIL2.h>

class Texture
{
public:
	Texture(const std::string& imagePath);
	~Texture();

	void Bind(int unitTexture = 0) const;
	void UnBind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getChannels() const { return m_Channals; }

private:
	unsigned int m_Texture;

	unsigned char * m_Image;

	int m_Width, m_Height, m_Channals;
};

