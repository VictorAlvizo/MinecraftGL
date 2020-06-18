#include "Texture.h"

Texture::Texture(const std::string& imagePath) {
	std::string fileName = imagePath.substr(imagePath.find('/') + 1, imagePath.length() - 1); //Get file name only

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_Image = SOIL_load_image(imagePath.c_str(), &m_Width, &m_Height, &m_Channals, SOIL_LOAD_RGBA);

	if (m_Image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Image: \"" << fileName << "\" load FAILED" << std::endl;
	}

	SOIL_free_image_data(m_Image);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_Texture);
}

void Texture::Bind(int unitTexture) const {
	glActiveTexture(GL_TEXTURE0 + unitTexture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::UnBind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}