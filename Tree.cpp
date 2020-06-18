#include "Tree.h"

Tree::Tree(glm::vec3 rootPos)
	:m_Wood(glm::vec3(0.0f)), m_Leaves(glm::vec3(0.0f))
{
	m_RootPos = rootPos;

	for (int i = 0; i < 5; i++) {
		m_LogPos.push_back(glm::vec3(m_RootPos.x, m_RootPos.y + i, m_RootPos.z));
	}
}

Tree::~Tree() {
	//holder
}

void Tree::Draw(Shader * genericShader, Shader * leavesShader) {
	leavesShader->UnBind();
	genericShader->Bind();

	//Drawing the vertical logs
	for (int i = 0; i < 5; i++) {
		m_Wood.SetPos(glm::vec3(m_RootPos.x, m_RootPos.y + i, m_RootPos.z));
		m_Wood.Draw(genericShader);
	}

	genericShader->UnBind();
	leavesShader->Bind();

	//Draw first layer of leaves
	for (int i = m_RootPos.z - 2; i < m_RootPos.z + 3; i++) {
		for (int j = m_RootPos.x - 2; j < m_RootPos.x + 3; j++) {
			if (j != m_RootPos.x || i != m_RootPos.z) { //Don't draw over existing log
				m_Leaves.SetPos(glm::vec3(j, m_RootPos.y + 2, i));
				m_Leaves.Draw(leavesShader);
			}
		}
	}

	//Draw second layer of leaves
	for (int i = m_RootPos.z - 2; i < m_RootPos.z + 3; i++) {
		for (int j = m_RootPos.x - 2; j < m_RootPos.x + 3; j++) {
			//Don't draw over existing log and skip corners
			if ((j != m_RootPos.x || i != m_RootPos.z) && (j != m_RootPos.x - 2 || i != m_RootPos.z - 2) &&
				(j != m_RootPos.x + 2 || i != m_RootPos.z + 2) && (j != m_RootPos.x - 2 || i != m_RootPos.z + 2) && (j != m_RootPos.x + 2 || i != m_RootPos.z - 2)) {
				m_Leaves.SetPos(glm::vec3(j, m_RootPos.y + 3, i));
				m_Leaves.Draw(leavesShader);
			}
		}
	}

	//Draw third layer of leaves
	for (int i = m_RootPos.z - 1; i < m_RootPos.z + 2; i++) {
		for (int j = m_RootPos.x - 1; j < m_RootPos.x + 2; j++) {
			if (j != m_RootPos.x || i != m_RootPos.z) {
				m_Leaves.SetPos(glm::vec3(j, m_RootPos.y + 4, i));
				m_Leaves.Draw(leavesShader);
			}
		}
	}

	//Draw fourth layer of leaves
	for (int i = m_RootPos.z - 1; i < m_RootPos.z + 2; i++) {
		for (int j = m_RootPos.x - 1; j < m_RootPos.x + 2; j++) {
			if ((j != m_RootPos.x - 1 || i != m_RootPos.z - 1) && (j != m_RootPos.x + 1 || i != m_RootPos.z + 1) &&
				(j != m_RootPos.x - 1 || i != m_RootPos.z + 1) && (j != m_RootPos.x + 1 || i != m_RootPos.z - 1)) {
				m_Leaves.SetPos(glm::vec3(j, m_RootPos.y + 5, i));
				m_Leaves.Draw(leavesShader);
			}
		}
	}

	//Time to top it of :)
	m_Leaves.SetPos(glm::vec3(m_RootPos.x, m_RootPos.y + 6, m_RootPos.z));
	m_Leaves.Draw(leavesShader);

	//Reset back to generic shaders as leaving the leaves shaders can interfere with other the other's blocks shaders
	leavesShader->UnBind();
	genericShader->Bind();
}

void Tree::SetRoot(glm::vec3 newPos) {
	m_RootPos = newPos;
}