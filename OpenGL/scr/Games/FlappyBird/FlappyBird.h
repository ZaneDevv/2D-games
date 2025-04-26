#pragma once

#include "../General/Game.h"
#include "../../DebugHelper/Printing.h"

class FlappyBird : public Game
{
private:

	const float GRAVITY = -0.1f;
	float VerticalVelocity = GRAVITY;
	
	const float JumpPower = 5.0f;

	Sprite* Bird = nullptr;


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

	/*
	* @brief Executes when a input is detected
	* @param Input key
	* @author ZaneDevv
	*/
	void OnInputDetected(int) override;
};