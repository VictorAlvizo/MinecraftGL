#pragma once
#include <vector>
#include "Blocks/WoodBlock.h"
#include "Blocks/OakleavesBlock.h"

class Tree {
public:
	Tree(glm::vec3 rootPos);
	~Tree();

	void Draw(Shader * genericShader, Shader * leavesShader);

	void SetRoot(glm::vec3 newPos);

	inline std::vector<glm::vec3> getLogs() const { return m_LogPos; }

private:
	std::vector<glm::vec3> m_LogPos;

	glm::vec3 m_RootPos;

	WoodBlock m_Wood;
	OakleavesBlock m_Leaves;
};

