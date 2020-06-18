#pragma once
#include <GL/glew.h>
#include <vector>
#include <iostream>

class VBO //Little bit of a misleading name as this class also contains both the IBO and VAO
{
public:
	VBO(const void * data, unsigned int size, const unsigned int * indices, unsigned int iboSize);
	~VBO();

	void Push(unsigned int count);

	void SetBuffers();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_Stride;

	std::vector<int> m_Vertices;
};

