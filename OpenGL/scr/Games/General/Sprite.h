#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../Libraries/stb_image.h"

#include "../../DebugHelper/Printing.h"
#include "../../DebugHelper/Settings.h"

struct Sprite {
private:

	float Scale = 1;
	float Angle = 0;
	float Position[3] = { 0, 0, 0 };

	/*
	* @brief Creates a quad so we can render images on screen
	* @author ZaneDevv
	*/
	void CreateQuad();

	/*
	* Loads a new image on screen
	* @param Path to the image
	* @author ZaneDevv
	*/
	GLuint LoadTexture(const char* path);

public:
	GLuint QuadVAO = 0;
	GLuint QuadVBO = 0;
	GLuint QuadEBO = 0;

	GLuint Image = 0;
	const char* Path = "";

	/*
	* @brief Creates a new sprite by the path of the image
	* @param Path to the image
	* @author ZaneDevv
	*/
	Sprite(const char*);

	/*
	* @brief Creates a new sprite by copying another one
	* @param Sprite to copy
	* @author ZaneDevv
	*/
	Sprite(const Sprite&);

	/*
	* @brief Destroies the sprites and frees the memory
	* @author ZaneDevv
	*/
	~Sprite();

	/*
	* @brief Rotates the sprite
	* @param Angle in radians
	* @author ZaneDevv
	*/
	void Rotate(float) const;

	/*
	* @brief Gets the position of the sprite
	* @return Position
	* @author ZaneDevv
	*/
	const float* GetPosition() const;

	/*
	* @brief Gets the rotation of the sprite
	* @return Angle of the rotation
	* @author ZaneDevv
	*/
	float GetRotation() const;

	/*
	* @brief Gets the scale of the sprite
	* @return Scale
	* @author ZaneDevv
	*/
	float GetScale() const;
};