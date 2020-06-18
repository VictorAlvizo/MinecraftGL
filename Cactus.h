#pragma once
#include <vector>
#include "Blocks/CactusBlock.h"

class Cactus
{
public:
	Cactus(glm::vec3 initPos, int cacHeight);
	~Cactus();
	
	void Draw(Shader * shader);

	void SetPos(glm::vec3 newPos);
	void SetHeight(int height);

	inline std::vector<glm::vec3> getBlocks() const { return m_CactusPos; }

private:
	std::vector<glm::vec3> m_CactusPos;

	glm::vec3 m_RootPos;

	CactusBlock m_CactusBlock;
	int m_CactusHeight;
};

