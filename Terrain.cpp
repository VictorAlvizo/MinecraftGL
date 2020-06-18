#include "Terrain.h"

Terrain::Terrain(int length)
	:m_MapLength(length)
{
	std::srand(time(NULL));
}

Terrain::~Terrain() {
	//holder
}

void Terrain::GenerateTerrain() {
	m_Terrain.clear();

	int biome = rand() % 2; //0: Plains, 1: Desert
	int cactusHeight = 0;

	int * terrainZ = new int[m_MapLength];

	for (int i = 0; i < m_MapLength; i++) { //Fill all values with 0 initally
		terrainZ[i] = 0;
	}

	float currentBlockY = 0.0f, lastBlockY = 0.0f;
	bool objectThere = false;

	for (int z = 0; z < m_MapLength; z++) {
		for (int x = 0; x < m_MapLength; x++) {
			objectThere = (rand() % 75 == 5) ? true : false; //Edit rand to increase or decrease # of trees

			currentBlockY = GetBlockY(lastBlockY, terrainZ[x]);
			lastBlockY = currentBlockY;
			terrainZ[x] = currentBlockY;

			if (biome == 0) { //Plains Section
				//Add dirt below grass block 
				for (int i = 0; i < currentBlockY; i++) {
					m_Terrain.push_back({ glm::vec3(x, i, z), BlockIdentifer::DIRT, false, false, 0});
				}

				m_Terrain.push_back({ glm::vec3(x, currentBlockY, z), BlockIdentifer::GRASS, objectThere, false, 0});

				if (objectThere) { //Just to get log positions for collison
					Tree tempTree(glm::vec3(x, currentBlockY + 1, z));

					for (glm::vec3 logPos : tempTree.getLogs()) {
						m_Terrain.push_back({ logPos, BlockIdentifer::OBJECT, false, true, 0});
					}
				}
			}
			else { //Desert Section
				for (int i = 0; i < currentBlockY; i++) {
					m_Terrain.push_back({ glm::vec3(x, i, z), BlockIdentifer::SAND, false, false, 0});
				}

				cactusHeight = rand() % 3 + 1;
				m_Terrain.push_back({ glm::vec3(x, currentBlockY, z), BlockIdentifer::SAND, objectThere, false, cactusHeight});

				if (objectThere) {
					Cactus tempCactus(glm::vec3(x, currentBlockY + 1, z), cactusHeight);

					for (glm::vec3 logPos : tempCactus.getBlocks()) {
						m_Terrain.push_back({ logPos, BlockIdentifer::OBJECT, false, true, 0});
					}
				}
			}
		}
	}

	delete[] terrainZ;
}

float Terrain::GetBlockY(float lastGrassY, float lastGrassZ) {
	int pointChange = rand() % 100; //Increasing the range will make the land flatter

	switch (pointChange) {
	case 0: //Go down
		return (lastGrassY > 0.0f && (lastGrassY - 1) == (lastGrassZ - 1)) ? --lastGrassY : lastGrassY; //Second argument is to not create large gaps between rows
		break;

	case 1: //Go up
		return (lastGrassY < 5.0f && (lastGrassY + 1) == (lastGrassZ + 1)) ? ++lastGrassY : lastGrassY;
		break;

	default: //Stay the same
		return lastGrassY;
		break;
	}
}

void Terrain::Add(BlockIdentifer blockID, glm::vec3 addPos) {
	m_Terrain.push_back({ addPos, blockID, false, false, 0});
}

void Terrain::Remove(int index) {
	m_Terrain.erase(m_Terrain.begin() + index);
}

int Terrain::isFound(glm::vec3 blockPos) {
	for (unsigned int i = 0; i < m_Terrain.size(); i++) {
		if (m_Terrain[i].pos == blockPos) {
			return i;
		}
	}

	return -1;
}
