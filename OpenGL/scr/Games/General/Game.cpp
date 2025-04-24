#include "Game.h"

void Game::CreateImage(const char* path) {
	/*GLuint image;
	glGenTextures(1, &image);
 	glBindTexture(GL_TEXTURE_2D, image);

 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 
 	int width, height, nrChannels = 0;
 	stbi_set_flip_vertically_on_load(true);

 	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

 	if (data) {
 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
 		ERROR_PRINT("Failed to load texture");
 	}
 	stbi_image_free(data);

 	if (DEBUGGING) {
 		DEBUG_PRINT("Image created!");
	}

	this->images.push_back(image);*/
}

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

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);

	return shaderProgram;
}

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

	// Updating the whole code
	while (!glfwWindowShouldClose(this->window)) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(this->shaderProgram);

		glfwPollEvents();

		for (GLuint image : this->images) {
			glBindTexture(GL_TEXTURE_2D, image);
		}

		this->Update(deltaTime);

		glfwSwapBuffers(this->window);
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