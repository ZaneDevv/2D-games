#pragma once

#include "../General/Game.h"
#include "../../DebugHelper/Printing.h"

#include "../../Libraries/stb_image.h"


class FlappyBird : public Game
{
public:
	/*
	* @brief Sets the value of the setting for this game once the game is created
	* @author ZaneDevv
	*/
	FlappyBird();

	/*
	* @brief Executes when the screen is created
	* @author ZaneDevv
	*/
	void OnScreenCreated() override;

	/*
	* @brief Updates the whole game
	* @author ZaneDevv
	*/
	void Update(double dt) override;

	/*
	* @brief Updates physics
	* @author ZaneDevv
	*/
	void UpdatePhysics();
};