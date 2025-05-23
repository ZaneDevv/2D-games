﻿#include <iostream>

#include "DebugHelper/Printing.h"
#include "DebugHelper/Settings.h"

#include "Games/FlappyBird/FlappyBird.h"

int main() {
	if (DEBUGGING) {
		DEBUG_PRINT("Starting game choice.");
	}

	Game* choseGame = nullptr;
	unsigned short choseGameIndex = 0;

	while (choseGameIndex == 0) {
		PRINT("Which game do you want to play? \n - Flappy Bird (1).");

		std::cout << ">> ";
		std::cin >> choseGameIndex;
	}

	switch (choseGameIndex) {
	case 1: choseGame = new FlappyBird(); break;

	default:
		if (DEBUGGING) {
			ERROR_PRINT("Game not found!");
		}

		return FAILED;
	}

	if (choseGame == nullptr) {
		if (DEBUGGING) {
			ERROR_PRINT("Game not found!");
		}

		return FAILED;
	}

	choseGame->CreateWindow();
	choseGame->StartLoop();


	delete choseGame;

	std::cin.get();
	return SUCCESS;
}
