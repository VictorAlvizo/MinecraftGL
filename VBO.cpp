#include "VBO.h"

VBO::VBO(const void * data, unsigned int size, const unsigned int * indices, unsigned int iboSize) {
	m_Stride = 0;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, indices, GL_STATIC_DRAW);
}

VBO::~VBO() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void VBO::Push(unsigned int count) {
	m_Vertices.push_back(count);
	m_Stride += count * sizeof(float);
}

void VBO::SetBuffers() {
	unsigned int offSet = 0;

	for (unsigned int i = 0; i < m_Vertices.size(); i++) {
		glVertexAttribPointer(i, m_Vertices[i], GL_FLOAT, GL_FALSE, m_Stride, (const void *)(offSet));
		glEnableVertexAttribArray(i);

		offSet += m_Vertices[i] * sizeof(float);
	}

	Unbind();
}

void VBO::Bind() const {
	glBindVertexArray(m_VAO);
}

void VBO::Unbind() const {
	glBindVertexArray(0);
}
