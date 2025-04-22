#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

#include "../../DebugHelper/Printing.h"
#include "../../DebugHelper/Settings.h"

class Game
{
protected:
	int screenWidth = 0;
	int screenHeight = 0;

	const char* screenName = "";

	GLFWwindow* window = nullptr;

public:
	/*
	* @brief Executes when the screen is created
	* @author ZaneDevv
	*/
	virtual void OnScreenCreated() = 0;

	/*
	* @brief Creates a window where the game will render.
	* @author ZaneDevv
	*/
	void CreateWindow();

	/*
	* @brief Updates the whole game
	* @param Delta time
	* @author ZaneDevv
	*/
	virtual void Update(double) = 0;

	/*
	* @brief Updates graphics
	* @author ZaneDevv
	*/
	virtual void UpdateGraphics() = 0;

	/*
	* @brief Starts the game
	* @author ZaneDevv
	*/
	void StartLoop();

	/*
	* @brief Closes the game
	* @author ZaneDevv
	*/
	void CloseGame();
};