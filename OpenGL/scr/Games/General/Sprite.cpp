#include "Sprite.h"

#pragma warning(push)
#pragma warning(disable : 6001)
#pragma warning(disable : 6001)
#pragma warning(disable : 26451)

#define STB_IMAGE_IMPLEMENTATION
#include "../../Libraries/stb_image.h"

#pragma warning(pop)

//------------------------ CONSTRUCTORS & DESTRUCTORS ------------------------//

Sprite::Sprite() {
	if (DEBUGGING) {
		WARNING_PRINT("Empty sprite created.");
	}
}

Sprite::Sprite(const char* path) {
	if (path == NULL) {

		if (DEBUGGING) {
			WARNING_PRINT("Path to the image null!");
		}

		path = " ";
	}

	this->Path = path;
	this->Image = this->LoadTexture(path);

	this->CreateQuad();
	if (this->QuadVAO == 0 || this->QuadVBO == 0 || this->QuadEBO == 0) {

		if (DEBUGGING) {
			WARNING_PRINT("Sprite will not be rendered!");
		}

		return;
	}

	if (DEBUGGING) {
		SUCCESS_PRINT("Image created correctly!");
	}
}

Sprite::Sprite(const Sprite& sprite) {
	this->Path = sprite.Path;

	if (this->Path == NULL) {

		if (DEBUGGING) {
			WARNING_PRINT("Path to the image null!");
		}

		this->Path = " ";
	}

	this->Image = this->LoadTexture(this->Path);

	this->Position[0] = sprite.GetPosition()[0];
	this->Position[1] = sprite.GetPosition()[1];

	this->Angle = sprite.GetRotation();
	this->Size = sprite.GetScale();


	this->CreateQuad();
	if (this->QuadVAO == 0 || this->QuadVBO == 0 || this->QuadEBO == 0) {

		if (DEBUGGING) {
			WARNING_PRINT("Sprite will not be rendered!");
		}

		return;
	}

	if (DEBUGGING) {
		SUCCESS_PRINT("Image created by copy correctly!");
	}
}

Sprite::~Sprite() {
	if (DEBUGGING) {
		DEBUG_PRINT("Removing sprite");
	}

	if (this->QuadVAO != 0) {
		glDeleteVertexArrays(1, &this->QuadVAO);
		this->QuadVAO = 0;
	}

	if (this->QuadVBO != 0) {
		glDeleteBuffers(1, &this->QuadVBO);
		this->QuadVBO = 0;
	}

	if (this->QuadEBO != 0) {
		glDeleteBuffers(1, &this->QuadEBO);
		this->QuadEBO = 0;
	}

	if (this->Image != 0) {
		glDeleteTextures(1, &this->Image);
		this->Image = 0;
	}

	if (DEBUGGING) {
		PRINT(BG_RED << "Sprite removed!" << RESET_COLOR);
	}
}



//------------------------ RENDER FUNCTIONS ------------------------//

void Sprite::CreateQuad() {
	float vertices[] = {
		// Positions           Texture coordinates
		this->TopLeftVertex[0], this->TopLeftVertex[1], 0.0f,  1.0f, 1.0f, // Top - left
		this->BottomLeftVertex[0], this->BottomLeftVertex[1], 0.0f,  1.0f, 0.0f, // Bottom - left
		this->BottomRightVertex[0],  this->BottomRightVertex[1], 0.0f,  0.0f, 0.0f, // Bottom - right
		this->TopRightVertex[0],  this->TopRightVertex[1], 0.0f,  0.0f, 1.0f,  // Top - right
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &this->QuadVAO);
	glGenBuffers(1, &this->QuadVBO);
	glGenBuffers(1, &this->QuadEBO);

	if (this->QuadVAO == 0 || this->QuadVBO == 0 || this->QuadEBO == 0) {

		if (DEBUGGING) {
			ERROR_PRINT("OpenGL buffers not initialized!");
		}

		return;
	}

	glBindVertexArray(this->QuadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->QuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

GLuint Sprite::LoadTexture(const char* path) {
	GLuint texture = 0;

	glGenTextures(1, &texture);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR && DEBUGGING) {
		ERROR_PRINT("OpenGL error after glGenTextures: " << error);
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	error = glGetError();
	if (error != GL_NO_ERROR && DEBUGGING) {
		ERROR_PRINT("OpenGL error after glBindTexture: " << error);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels = 0;
	unsigned char* data = stbi_load(path, &this->width, &this->height, &nrChannels, 0);

	if (!data) {

		if (DEBUGGING) {
			ERROR_PRINT("Texture loading failed: " << path);
		}

		stbi_image_free(data);
		return texture;
	}

	if (DEBUGGING) {
		DEBUG_PRINT("Image loaded: " << std::endl << "- width: " << this->width << std::endl << "- height: " << this->height);
	}

	// Checking if the GPU can work with the size of the image
	GLint maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	if (this->width > maxTextureSize || this->height > maxTextureSize) {
		ERROR_PRINT("Texture exceeds max allowed size: " << this->width << "x" << this->height);
		stbi_image_free(data);
		return texture;
	}

	// Checking the channels the image needs
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;

	else if (nrChannels == 3)
		format = GL_RGB;

	else if (nrChannels == 4)
		format = GL_RGBA;
	else {

		if (DEBUGGING) {
			ERROR_PRINT("Unsupported image format -> Only RGB or RGBA allowed.");
		}
		
		
		stbi_image_free(data);
		return texture;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}



//------------------------ TRANSFORM ------------------------//

void Sprite::UpdateTransform() {
	// Calculating the new position of each vertex
	float size = this->Size * .005f;
	float cosine = (float)cos(this->Angle);
	float sine = (float)sin(this->Angle);

	float dx[4] = { -size * this->width, -size * this->width, size * this->width, size * this->width };
	float dy[4] = { size * this->height, -size * this->height, -size * this->height, size * this->height };

	this->TopLeftVertex[0] = this->Position[0] + dx[0] * cosine - dy[0] * sine;
	this->TopLeftVertex[1] = this->Position[1] + dx[0] * sine + dy[0] * cosine;

	this->BottomLeftVertex[0] = this->Position[0] + dx[1] * cosine - dy[1] * sine;
	this->BottomLeftVertex[1] = this->Position[1] + dx[1] * sine + dy[1] * cosine;

	this->BottomRightVertex[0] = this->Position[0] + dx[2] * cosine - dy[2] * sine;
	this->BottomRightVertex[1] = this->Position[1] + dx[2] * sine + dy[2] * cosine;

	this->TopRightVertex[0] = this->Position[0] + dx[3] * cosine - dy[3] * sine;
	this->TopRightVertex[1] = this->Position[1] + dx[3] * sine + dy[3] * cosine;

	this->CreateQuad();
}

void Sprite::Rotate(double theta) {
	this->Angle += theta;
	this->UpdateTransform();
}

void Sprite::Translate(const float* move) {
	if (move == nullptr) {

		if (DEBUGGING) {
			ERROR_PRINT("Cannot move null");
		}

		return;
	}

	this->Position[0] += move[0] / this->width;
	this->Position[1] += move[1] / this->height;
	this->UpdateTransform();
}

void Sprite::Scale(float scale) {
	this->Size = scale;
	this->UpdateTransform();
}



//------------------------ GETTERS & SETTERS ------------------------//

const float* Sprite::GetPosition() const {
	return this->Position;
}

double Sprite::GetRotation() const {
	return this->Angle;
}

float Sprite::GetScale() const {
	return this->Size;
}

void Sprite::SetPosition(const float* position) {
	if (position == nullptr) {

		if (DEBUGGING) {
			ERROR_PRINT("Cannot set a null pointer as the position.");
		}

		return;
	}

	this->Position[0] = position[0] / this->width;
	this->Position[1] = position[1] / this->height;

	this->UpdateTransform();
}

void Sprite::SetScale(float size) {
	this->Size = size;
	this->UpdateTransform();
}

void Sprite::SetRotation(double theta) {
	this->Angle = theta;
	this->UpdateTransform();
}