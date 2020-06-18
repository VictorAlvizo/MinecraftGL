#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "Blocks/GrassBlock.h"
#include "Blocks/DirtBlock.h"
#include "Blocks/SandBlock.h"
#include "Blocks/BrickBlock.h"
#include "Blocks/PlankBlock.h"
#include "Blocks/GlassBlock.h"
#include "Tree.h"
#include "Cactus.h"

enum class BlockIdentifer {
	OBJECT, GRASS, DIRT, SAND, BRICK, PLANK, GLASS
};

struct TerrainBlock {
	glm::vec3 pos;
	BlockIdentifer blockID;
	bool objectHere;
	bool hidden;
	int cacHeight;
};

class Terrain
{
public:
	Terrain(int length);
	~Terrain();

	void GenerateTerrain();

	void Add(BlockIdentifer blockID, glm::vec3 addPos);
	void Remove(int removeIndex);

	int isFound(glm::vec3 blockPos);

	inline std::vector<TerrainBlock> getTerrain() const { return m_Terrain; }
	inline int getLength() const { return m_MapLength; }

	TerrainBlock operator[](int index) {
		return m_Terrain[index];
	}

private:
	float GetBlockY(float lastGrassY, float lastGrassZ);

	std::vector<TerrainBlock> m_Terrain;

	int m_MapLength; //Map width and length values are always going to be the same
};

