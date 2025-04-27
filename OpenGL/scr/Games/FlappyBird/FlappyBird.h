#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <array>
#include <algorithm>

#include "../General/Game.h"
#include "../../DebugHelper/Printing.h"

#define PI 3.141592653589793

class FlappyBird : public Game
{
private:
	bool hasGameStarted = false;
	bool hasGameFinished = false;
	double timePlaying = 0;

	const float PipeHorizontalSpeed = 20.0f;

	const float GRAVITY = -10.0f;
	float VerticalVelocity = GRAVITY;
	
	const float JumpPower = 300.0f;
	const double LimitAngle = PI * .25f;

	const int gapVerticalSize = 145;
	const int distanceBetweenPipes = 50;
	const int startingPipesXPosition = 75;
	const float pipesScale = 0.75f;

	Sprite* Bird = nullptr;

	std::vector<Sprite*> Pipes = {};


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
	* @param delta time
	* @author ZaneDevv
	*/
	void UpdatePhysics(double);

	/*
	* @brief Checks if the bird has collided with a pipe
	* @param Reference to the pipe to check
	* @author ZaneDevv
	*/
	bool DoesBirdOverlapAPipe(Sprite*);

	/*
	* @brief Kills the bird and stops the game
	* @author ZaneDevv
	*/
	void Die();

	/*
	* @brief Executes when a input is detected
	* @param Input key
	* @author ZaneDevv
	*/
	void OnInputDetected(int) override;

	/*
	* @brief Fires when the game is closed
	* @author ZaneDevv
	*/
	void OnGameClose() override;
};