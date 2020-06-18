#include "Cactus.h"

Cactus::Cactus(glm::vec3 initPos, int cacHeight) 
	:m_CactusBlock(glm::vec3(0.0f))
{
	m_RootPos = initPos;
	m_CactusHeight = cacHeight;

	for (int i = 0; i < m_CactusHeight; i++) { //Just to get collison blocks
		m_CactusPos.push_back(glm::vec3(m_RootPos.x, m_RootPos.y + i, m_RootPos.z));
	}
}

Cactus::~Cactus(){
	//holder
}

void Cactus::Draw(Shader * shader) {
	shader->Bind();

	for (int i = 0; i < m_CactusHeight; i++) {
		m_CactusBlock.SetPos(glm::vec3(m_RootPos.x, m_RootPos.y + i, m_RootPos.z));
		m_CactusBlock.Draw(shader);
	}

	shader->UnBind();
}

void Cactus::SetPos(glm::vec3 newPos) {
	m_RootPos = newPos;
}

void Cactus::SetHeight(int height) {
	m_CactusHeight = height;
}
