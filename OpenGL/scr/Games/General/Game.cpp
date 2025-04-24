#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Libraries/stb_image.h"

void Game::CreateQuad() {
	float vertices[] = {
		// Positions         Texture coordinates
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Top - left
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom - left
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom - right
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f  // Top - right
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &this->quadVBO);
	glGenBuffers(1, &this->quadEBO);

	if (this->quadVAO == 0 || this->quadVBO == 0 || this->quadEBO == 0) {

		if (DEBUGGING) {
			ERROR_PRINT("OpenGL buffers not initialized!");
		}

		return;
	}


	if (DEBUGGING) {
		DEBUG_PRINT("Buffers initializated");
	}

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

GLuint Game::LoadTexture(const char* path) {
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (!data) {

		if (DEBUGGING) {
			ERROR_PRINT("Texture loading failed: " << path);
		}

		stbi_image_free(data);
		return texture;
	}

	// Checking if the BPU can work with the size of the image
	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	if (width > maxTextureSize || height > maxTextureSize) {
		ERROR_PRINT("Texture exceeds max allowed size: " << width << "x" << height);
		stbi_image_free(data);
		return texture;
	}

	// Checking if the image has a supported number of channels
	if (nrChannels != 3 && nrChannels != 4) {
		ERROR_PRINT("Unsupported image format -> Only RGB or RGBA allowed.");
		stbi_image_free(data);
		return texture;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}

void Game::CreateImage(const char* path) {
	GLuint texture = LoadTexture(path);
	Sprite sprite(texture);
	this->images.push_back(sprite);
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	this->CreateQuad();
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

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (const Sprite& image : this->images) {
		glBindTexture(GL_TEXTURE_2D, image.Image);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void Game::CloseGame() {
	glfwDestroyWindow(this->window);
	glfwTerminate();

	if (DEBUGGING) {
		DEBUG_PRINT("Game closed correctly!");
	}
}