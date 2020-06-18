#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <mutex>
#include "Shader.h"
#include "VBO.h"
#include "Camera.h"
#include "Terrain.h"
#include "Skybox.h"
#include "MousePicker.h"

void ProcessInput(GLFWwindow* window);
void Grounded();
void BlockAdd();
bool MouseRayDetection(glm::vec3 mouseRay);
void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
bool isValid(Direction dir);
void CollisionRay(glm::vec3 frontVector, Direction dir, int& xAdd, int& zAdd);
void TerrainWatch();
bool PlayerAtEdge(int& xSpawn, int& zSpawn);
void SpawnPlayer(const int spawnX, const int spawnZ);

static void FrameBufferCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Camera camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
glm::mat4 proj, view;

float deltaTime = 0.0f, lastTime = 0.0f;
double mouseX, mouseY;
int winWidth, winHeight;

Terrain terrain(25);
std::mutex terCamMutex; //Mutex for both the camera and terrain

int selectedBlock; //Index of the selected block
BlockIdentifer currentBlock = BlockIdentifer::DIRT; //When adding what block will show up

int main() {
	GLFWwindow * window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //GLFW Version 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwInit()) {
		std::cout << "GLFW did not work" << std::endl;
	}
	
	//Default window size
	winWidth = 640;
	winHeight = 480;

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	winWidth = mode->width;
	winHeight = mode->height;

	std::cout << "Moniter Dimesions: " << "(" << winWidth << "," << winHeight << ")" << std::endl;
	std::cout << "Refresh Rate: " << mode->refreshRate << std::endl;

	window = glfwCreateWindow(winWidth, winHeight, "MinecraftGL", NULL, NULL);

	if (!window)
	{
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "Something went wrong with GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, FrameBufferCallback);
	glfwGetFramebufferSize(window, &winWidth, &winHeight);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, MousePosCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	Shader leavesShader("src/Shaders/VertexShader.glsl", "src/Shaders/LeavesShader.glsl");

	Shader skyboxShader("src/Shaders/SkyboxVertex.glsl", "src/Shaders/SkyboxFragment.glsl");
	skyboxShader.Bind();
	skyboxShader.SetInt("u_Skybox", 0);

	view = glm::mat4(1.0f);
	proj = glm::mat4(1.0f);
	glm::mat4 skyboxView = glm::mat4(1.0f);

	terrain.GenerateTerrain();
	SpawnPlayer((int)terrain.getLength() / 2, (int)terrain.getLength() / 2);

	std::thread terrainThread(TerrainWatch);
	terrainThread.detach();

	GrassBlock grassBlock(glm::vec3(0.0f));
	DirtBlock dirtBlock(glm::vec3(0.0f));
	SandBlock sandBlock(glm::vec3(0.0f));
	BrickBlock brickBlock(glm::vec3(0.0f));
	PlankBlock plankBlock(glm::vec3(0.0f));
	GlassBlock glassBlock(glm::vec3(0.0f));

	Tree tree(glm::vec3(0.0f));
	Cactus cactus(glm::vec3(0.0f), 0); //This soley decides cactus height

	std::vector<std::string> skyboxFiles;
	skyboxFiles.push_back("Textures/Skybox/right.jpg");
	skyboxFiles.push_back("Textures/Skybox/left.jpg");
	skyboxFiles.push_back("Textures/Skybox/top.jpg");
	skyboxFiles.push_back("Textures/Skybox/bottom.jpg");
	skyboxFiles.push_back("Textures/Skybox/front.jpg");
	skyboxFiles.push_back("Textures/Skybox/back.jpg");

	Skybox skybox(skyboxFiles);
	float dayColor = 1.0f; //For daylight / nighttime change sky color
	bool colorDown = true; //Is the color value currently set to go down

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::mat4(1.0f);
		view = camera.UpdateView();
		skyboxView = glm::mat4(glm::mat3(view));

		proj = glm::perspective(glm::radians(60.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);

		if (colorDown && dayColor <= 0.3f) {
			colorDown = false;
			dayColor = 0.5f;
		}
		else if (!colorDown && dayColor >= 1.0f) {
			colorDown = true;
			dayColor = 1.0f;
		}

		dayColor += (colorDown) ? -0.001f : 0.001f;

		skyboxShader.Bind();
		skyboxShader.SetMat4fv("u_View", skyboxView);
		skyboxShader.SetMat4fv("u_Projection", proj);
		skyboxShader.SetFloat("u_BlueColor", dayColor);
		
		skybox.Draw();
		skyboxShader.UnBind();

		leavesShader.Bind();
		leavesShader.SetMat4fv("u_View", view);
		leavesShader.SetMat4fv("u_Projection", proj);
		leavesShader.SetFloat("u_Time", glfwGetTime());
		leavesShader.UnBind();

		shader.Bind();
		shader.SetMat4fv("u_View", view);
		shader.SetMat4fv("u_Projection", proj);

		{ //Needed to create a custom scope to avoid trying to get terrain data when the terrain thread is clearing it
			std::lock_guard<std::mutex> lock(terCamMutex);

			for (unsigned int i = 0; i < terrain.getTerrain().size(); i++) {
				if (!terrain[i].hidden) {
					//Tried to shortern this using block pointers but the game became unplayable :(. Had to resort to this
					if (terrain[i].blockID == BlockIdentifer::GRASS) {
						grassBlock.SetPos(terrain[i].pos);
						grassBlock.Draw(&shader);
					}
					else if (terrain[i].blockID == BlockIdentifer::DIRT) {
						dirtBlock.SetPos(terrain[i].pos);
						dirtBlock.Draw(&shader);
					}
					else if (terrain[i].blockID == BlockIdentifer::SAND) {
						sandBlock.SetPos(terrain[i].pos);
						sandBlock.Draw(&shader);
					}
					else if (terrain[i].blockID == BlockIdentifer::BRICK) {
						brickBlock.SetPos(terrain[i].pos);
						brickBlock.Draw(&shader);
					}
					else if (terrain[i].blockID == BlockIdentifer::PLANK) {
						plankBlock.SetPos(terrain[i].pos);
						plankBlock.Draw(&shader);
					}
					else if (terrain[i].blockID == BlockIdentifer::GLASS) {
						glassBlock.SetPos(terrain[i].pos);
						glassBlock.Draw(&shader);
					}

					//Check for objects
					if (terrain[i].objectHere && terrain[i].blockID == BlockIdentifer::GRASS) {
						tree.SetRoot(glm::vec3(terrain[i].pos.x, terrain[i].pos.y + 1, terrain[i].pos.z));
						tree.Draw(&shader, &leavesShader);
					}
					else if (terrain[i].objectHere && terrain[i].blockID == BlockIdentifer::SAND) {
						cactus.SetHeight(terrain[i].cacHeight);
						cactus.SetPos(glm::vec3(terrain[i].pos.x, terrain[i].pos.y + 1, terrain[i].pos.z));
						cactus.Draw(&shader);
					}
				}
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void ProcessInput(GLFWwindow * window) {
	std::lock_guard<std::mutex> lock(terCamMutex);
	std::this_thread::sleep_for(std::chrono::milliseconds(40)); //This is in place to avoid keys being inputted twice+ without a g

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float curTime = glfwGetTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;

	//Camera movement input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && isValid(Direction::FOWARD)) {
		camera.Move(Direction::FOWARD, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && isValid(Direction::BACK)) {
		camera.Move(Direction::BACK, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && isValid(Direction::LEFT)) {
		camera.Move(Direction::LEFT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && isValid(Direction::RIGHT)) {
		camera.Move(Direction::RIGHT, deltaTime);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && terrain.isFound(glm::vec3((int)camera.getPos().x, (int)camera.getPos().y + 1, (int)camera.getPos().z)) == -1) { //Jump button
		camera.Jump();
		std::thread checkGround(Grounded);
		checkGround.detach();
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { //Toggle for camera freemode movement
		camera.SetFreemode();
	}

	for (int i = 49; i <= 54; i++) { //Check if 1 - 5 is pressed and set the block id accordingly 
		if (glfwGetKey(window, i) == GLFW_PRESS) {
			currentBlock = static_cast<BlockIdentifer>(i - 48); //Want to go from grass to plank and -48 as I never want to have an object selected
			break;
		}
	}

	//Mouse button input
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { //Add block
		MousePicker picker(camera, proj, mouseX, mouseY, winWidth, winHeight);

		if (MouseRayDetection(picker.getRay())) {
			BlockAdd();
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { //Remove block
		MousePicker picker(camera, proj, mouseX, mouseY, winWidth, winHeight);

		if (MouseRayDetection(picker.getRay())) {
			terrain.Remove(selectedBlock);
		}
	}
}

void Grounded() { //After jumping check if I landed on a new block
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	glm::vec3 playerPos = glm::vec3((int)camera.getPos().x, (int)camera.getPos().y, (int)camera.getPos().z); //Predicted future player pos

	if (terrain.isFound(glm::vec3(playerPos.x, playerPos.y - 2, playerPos.z)) != -1) {
		camera.SetJumpActive(true);
	}
}

void BlockAdd() {
	if (terrain.isFound(glm::vec3(terrain[selectedBlock].pos.x, terrain[selectedBlock].pos.y + 1, terrain[selectedBlock].pos.z)) == -1) {
		terrain.Add(currentBlock, glm::vec3(terrain[selectedBlock].pos.x, terrain[selectedBlock].pos.y + 1, terrain[selectedBlock].pos.z));
	}
	else {
		glm::vec3 frontVector = glm::normalize(glm::cross(camera.getUp(), camera.getRight()));
		frontVector.x = std::floor(frontVector.x + 0.5f);
		frontVector.y = std::floor(frontVector.y + 0.5f);
		frontVector.z = std::floor(frontVector.z + 0.5f);

		if (glm::vec3(0.0f, 0.0f, 1.0f) == frontVector || glm::vec3(1.0f, 0.0f, 1.0f) == frontVector || glm::vec3(-1.0f, 0.0f, 1.0f) == frontVector) { //Facing foward
			terrain.Add(currentBlock, glm::vec3(terrain[selectedBlock].pos.x, terrain[selectedBlock].pos.y, terrain[selectedBlock].pos.z - 1));
		}
		else if (glm::vec3(1.0f, 0.0f, 0.0f) == frontVector) { //Facing left
			terrain.Add(currentBlock, glm::vec3(terrain[selectedBlock].pos.x - 1, terrain[selectedBlock].pos.y, terrain[selectedBlock].pos.z));
		}
		else if (glm::vec3(0.0f, 0.0f, -1.0f) == frontVector) { //Facing back
			terrain.Add(currentBlock, glm::vec3(terrain[selectedBlock].pos.x, terrain[selectedBlock].pos.y, terrain[selectedBlock].pos.z + 1));
		}
		else { //Right
			terrain.Add(currentBlock, glm::vec3(terrain[selectedBlock].pos.x + 1, terrain[selectedBlock].pos.y, terrain[selectedBlock].pos.z));
		}
	}
}

bool MouseRayDetection(glm::vec3 mouseRay) {
	glm::vec3 addVector = mouseRay;
	mouseRay = glm::vec3(glm::round(camera.getPos().x), glm::round(camera.getPos().y), glm::round(camera.getPos().z));
	glm::vec3 updatedRay;

	int foundIndex = -1;
	bool blockFound = false;

	for (int i = 0; i < 10; i++) {
		updatedRay = glm::vec3(glm::round(mouseRay.x), round(mouseRay.y), round(mouseRay.z));
		foundIndex = terrain.isFound(updatedRay);

		if (foundIndex != -1) {
			selectedBlock = foundIndex;
			return true;
		}

		mouseRay += addVector;
	}

	selectedBlock = 0;
	return false;
}

void MousePosCallback(GLFWwindow * window, double xPos, double yPos) {
	std::lock_guard<std::mutex> lock(terCamMutex);
	camera.MouseMove(xPos, yPos);

	mouseX = xPos;
	mouseY = yPos;
}

bool isValid(Direction dir) {
	if (camera.freemodeActive()) {
		return true;
	}

	int xAdd = 0, zAdd = 0;
	bool validPos = true, jumpDown = true;

	glm::vec3 frontVector = glm::normalize(glm::cross(camera.getUp(), camera.getRight()));
	frontVector.x = std::floor(frontVector.x + 0.5f);
	frontVector.y = std::floor(frontVector.y + 0.5f);
	frontVector.z = std::floor(frontVector.z + 0.5f);

	CollisionRay(frontVector, dir, xAdd, zAdd);

	glm::vec3 playerPos = glm::vec3((int)camera.getPos().x + xAdd, (int)camera.getPos().y, (int)camera.getPos().z + zAdd); //Predicted future player pos

	if (terrain.isFound(playerPos) != -1 || terrain.isFound(glm::vec3(playerPos.x, playerPos.y - 1, playerPos.z)) != -1) {
		validPos = false;
	}

	if (terrain.isFound(glm::vec3(playerPos.x, playerPos.y - 2, playerPos.z)) != -1) {
		jumpDown = false;
	}

	if (jumpDown && validPos) {
		camera.Fall(camera.getPos().y - 1);
	}

	return validPos;
}

void CollisionRay(glm::vec3 frontVector, Direction dir, int& xAdd, int& zAdd) {
	if (glm::vec3(0.0f, 0.0f, 1.0f) == frontVector || glm::vec3(1.0f, 0.0f, 1.0f) == frontVector || glm::vec3(-1.0f, 0.0f, 1.0f) == frontVector) { //Facing foward
		switch (dir) {
			case Direction::FOWARD:
				zAdd = 1;
				break;

			case Direction::BACK:
				zAdd = -1;
				break;

			case Direction::LEFT:
				zAdd = 1;
				break;

			case Direction::RIGHT:
				zAdd = -1;
				break;
		}
	}
	else if (glm::vec3(1.0f, 0.0f, 0.0f) == frontVector) { //Facing left
		switch (dir) {
			case Direction::FOWARD:
				xAdd = 1;
				zAdd = 1;
				break;

			case Direction::BACK:
				xAdd = -1;
				break;

			case Direction::LEFT:
				zAdd = -1;
				break;

			case Direction::RIGHT:
				zAdd = 1;
				break;
		}
	}
	else if (glm::vec3(0.0f, 0.0f, -1.0f) == frontVector) { //Facing back
		switch (dir) {
			case Direction::FOWARD:
				zAdd = -1;
				break;

			case Direction::BACK:
				zAdd = 1;
				break;

			case Direction::LEFT:
				xAdd = -1;
				break;

			case Direction::RIGHT:
				xAdd = 1;
				zAdd = 1;
				break;
		}
	}
	else {
		switch (dir) { //Facing right
			case Direction::FOWARD:
				xAdd = -1;
				zAdd = 1;
				break;

			case Direction::BACK:
				xAdd = 1;
				zAdd = 1;
				break;

			case Direction::LEFT:
				zAdd = 1;
				break;

			case Direction::RIGHT:
				zAdd = -1;
				break;
		}
	}
}

void TerrainWatch() {
	int xSpawn = 0, zSpawn = 0;

	while (true) {
		if (PlayerAtEdge(xSpawn, zSpawn)) {
			std::lock_guard<std::mutex> lock(terCamMutex);

			terrain.GenerateTerrain();
			SpawnPlayer(xSpawn, zSpawn);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

bool PlayerAtEdge(int& xSpawn, int& zSpawn) { //If the player is in a position close to edge creating new terrain validated
	glm::vec3 playerPos = glm::vec3((int)camera.getPos().x, (int)camera.getPos().y, (int)camera.getPos().z);

	if (playerPos.z + 1 >= terrain.getLength()) { //Reaching northern edge
		xSpawn = playerPos.x;
		zSpawn = 4;

		return true;
	}
	else if (playerPos.z - 1 <= 0) { //Reaching southern edge
		xSpawn = playerPos.x;
		zSpawn = (int)terrain.getLength() - 4;

		return true;
	}
	else if (playerPos.x + 1 >= (int)terrain.getLength()) { //Reaching eastern edge
		xSpawn = 4;
		zSpawn = playerPos.z;

		return true;
	}
	else if (playerPos.x - 1 <= 0) { //Reaching western edge
		xSpawn = (int)terrain.getLength() - 4;
		zSpawn = playerPos.z;
		
		return true;
	}

	return false;
}

void SpawnPlayer(const int spawnX, const int spawnZ) {
	int validSpawnY = 0;

	//Spawn above the highest block at that cordinate
	for (unsigned int i = 0; i < terrain.getTerrain().size(); i++) {
		if (terrain[i].pos.x == spawnX && terrain[i].pos.z == spawnZ) {
			if (terrain[i].pos != glm::vec3(spawnX, validSpawnY, spawnZ)) {
				break;
			}

			validSpawnY++;
		}
	}

	camera.SetPos(glm::vec3(spawnX, validSpawnY + 1.0f, spawnZ));
}