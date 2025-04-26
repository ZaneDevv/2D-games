#pragma once

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../DebugHelper/Printing.h"
#include "../../DebugHelper/Settings.h"


struct Sprite {
private:

	float Size = 1;
	double Angle = 0;
	float Position[2] = { 0, 0 };

	float TopLeftVertex[2] = { -1.0f, 1.0f };
	float BottomLeftVertex[2] = { -1.0f, -1.0f };
	float BottomRightVertex[2] = { 1.0f, -1.0f };
	float TopRightVertex[2] = { 1.0f, 1.0f };

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

	/*
	* @brief Updates the transform of the sprite
	* @author ZaneDevv
	*/
	void UpdateTransform();

public:
	GLuint QuadVAO = 0;
	GLuint QuadVBO = 0;
	GLuint QuadEBO = 0;

	GLuint Image = 0;
	const char* Path = "";

	Sprite();

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
	void Rotate(double);

	/*
	* @brief Moves the sprite towards the given direction
	* @param Move vector
	* @author ZaneDevv
	*/
	void Translate(const float*);

	/*
	* @brief Scales the sprite
	* @param Amount
	* @author ZaneDevv
	*/
	void Scale(float);

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
	double GetRotation() const;

	/*
	* @brief Gets the scale of the sprite
	* @return Scale
	* @author ZaneDevv
	*/
	float GetScale() const;
};