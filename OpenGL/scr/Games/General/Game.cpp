#include "Game.h"

//------------------------ TEXTURE FUNCTIONS ------------------------//

Sprite* Game::CreateImage(const char* path) {
	if (DEBUGGING) {
		DEBUG_PRINT("Creating image");
	}

	this->images.push_back(new Sprite(path));
	return this->images.back();
}



//------------------------ SHADERS FUNCTIONS ------------------------//

GLuint Game::CompileShader(const char* source, GLenum type) {
	// Compiling shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Checking if it all worked correctly
	int success = 0;
	char errorMessage[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, errorMessage);
		ERROR_PRINT("Shader compilation failed: " << errorMessage);
	}
	else {
		if (DEBUGGING) {
			DEBUG_PRINT("Shader compiled successfully!");
		}
	}

	return shader;
}

GLuint Game::CreateShaderProgram() {
	// Creating vertex and fragment shaders
	GLuint vertexShader = this->CompileShader(this->vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fragmentShader = this->CompileShader(this->fragmentShaderCode, GL_FRAGMENT_SHADER);

	// Creating a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checking if it all worked correctly
	int success;
	char errorMessage[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, errorMessage);
		ERROR_PRINT("Program linking failed: " << errorMessage);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}



//------------------------ WINDOW FUNCTIONS ------------------------//

void Game::CreateWindow() {
	// Initializing glfw for the window
	if (!glfwInit()) {

		if (DEBUGGING) {
			ERROR_PRINT("Graphics library cannot load correctly.");
		}

		return;
	}

	// Creating window
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

	// Initializing glew to have modern OpenGL functions
	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();

	if (glewStatus != GLEW_OK) {

		if (DEBUGGING) {
			ERROR_PRINT("Glew cannot be initilized: " << glewGetErrorString(glewStatus));
		}

		return;
	}

	if (DEBUGGING) {
		SUCCESS_PRINT("Window created.");
	}

	// Calling functions when the window is successfully created
	this->shaderProgram = this->CreateShaderProgram();
	this->OnScreenCreated();
}

void Game::StartLoop() {
	// Ensuring the window exists
	if (this->window == nullptr) {

		if (DEBUGGING) {
			ERROR_PRINT("Window not specified.");
		}

		return;
	}

	// Calculating delta time
	double lastTime = glfwGetTime();
	double deltaTime = 0;

	if (DEBUGGING) {
		DEBUG_PRINT("Starting game.");
	}

	// Updating the whole code
	while (!glfwWindowShouldClose(this->window)) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		this->UpdateGraphics();
		this->Update(deltaTime);

		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	this->CloseGame();
}

void Game::UpdateGraphics() {
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(this->shaderProgram);

	for (Sprite* image : this->images) {
		glBindVertexArray(image->QuadVAO);
		glBindTexture(GL_TEXTURE_2D, image->Image);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void Game::CloseGame() {
	for (Sprite* image : this->images) {
		delete image;
	}

	glfwDestroyWindow(this->window);
	glfwTerminate();

	if (DEBUGGING) {
		DEBUG_PRINT("Game closed correctly!");
	}
}