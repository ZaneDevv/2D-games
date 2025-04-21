#include "Game.h"

void Game::CreateWindow() {
	if (!glfwInit()) {

		if (DEBUGGING) {
			ERROR_PRINT("Graphics library cannot load correctly.");
		}

		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	this->window = glfwCreateWindow(this->screenWidth, this->screenHeight, this->screenName, NULL, NULL);
	if (!this->window) {
		glfwTerminate();

		if (DEBUGGING) {
			ERROR_PRINT("Cannot create a window.");
		}

		return;
	}

	glfwMakeContextCurrent(this->window);

	if (DEBUGGING) {
		DEBUG_PRINT("Window created.");
	}

	this->OnScreenCreated();
}

void Game::StartLoop() {
	if (this->window == nullptr) {

		if (DEBUGGING) {
			ERROR_PRINT("Window not specified.");
		}

		return;
	}

	double lastTime = glfwGetTime();
	double deltaTime = 0;

	while (!glfwWindowShouldClose(this->window)) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);

		this->Update(deltaTime);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	this->CloseGame();
}

void Game::CloseGame() {
	glfwDestroyWindow(this->window);
	glfwTerminate();

	if (DEBUGGING) {
		DEBUG_PRINT("Game closed correctly!");
	}
}