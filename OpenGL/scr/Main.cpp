#include <iostream>

#include "Debug/Printing.h"
#include "Debug/Settings.h"

#include "Games/FlappyBird/FlappyBird.h"

int main() {
	if (DEBUGGING) {
		DEBUG_PRINT("Starting game choice.");
	}

	Game* choseGame = nullptr;
	unsigned short choseGameIndex = 0;

	while (choseGameIndex == 0) {
		PRINT("Which game do you want to play? \n 1. Flappy Bird.");

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

	std::cin.get();
	return SUCCESS;
}