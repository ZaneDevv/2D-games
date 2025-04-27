#include "FlappyBird.h"

// Assets got from https://github.com/samuelcust/flappy-bird-assets

FlappyBird::FlappyBird() {
	this->screenWidth = 432;
	this->screenHeight = 768;
	this->screenName = "Flappy bird";
}

void FlappyBird::OnScreenCreated() {
	// Adding the background
	Sprite* background = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/background-day.png");
	background->Rotate(PI);
	background->Scale(0.7f);

	// Adding the pipes
	for (short i = 0; i <= 4; i++) {
		float xPosition = (float)this->startingPipesXPosition + i * (float)this->distanceBetweenPipes;

		Sprite* pipe1 = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/pipe-green.png");
		pipe1->SetScale(this->pipesScale);

		Sprite* pipe2 = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/pipe-green.png");
		pipe2->Rotate(PI);
		pipe2->SetScale(this->pipesScale);

		this->SetPipesPosition(pipe1, pipe2, xPosition);

		std::array<Sprite*, 2> pipes = { pipe1, pipe2 };
		this->Pipes.push_back(pipes);
	}

	// Adding the bird
	float birdPosition[2]{ -200.0f, 0.0f };

	Sprite* bird = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/Bird.png");
	bird->Scale(.04f);
	bird->SetPosition(birdPosition);
	bird->Rotate(PI);

	this->Bird = bird;
}

void FlappyBird::Update(double dt) {
	if (!this->hasGameStarted) { return; }
	if (hasGameFinished) { return; }

	this->UpdatePhysics(dt);
	timePlaying += dt;

	if (timePlaying > 1.5) {
		float translation[2] = { (float)dt * -this->PipeHorizontalSpeed, 0.0f };

		for (std::array<Sprite*, 2> pipe : this->Pipes) {
			if (pipe[0] == nullptr || pipe[1] == nullptr) {
				
				if (DEBUGGING) {
					WARNING_PRINT("Null pipe!");
				}

				continue;
			}

			// Move both pipes
			pipe[0]->Translate(translation);
			pipe[1]->Translate(translation);

			// Check if the pipes should go back
			if (pipe[0]->GetPosition()[0] < -4) {
				this->SetPipesPosition(pipe[0], pipe[1], (float)this->startingPipesXPosition);

				if (DEBUGGING) {
					PRINT("Pipe moved");
				}
			}

			// Checking the collision with the bird
			if (DoesBirdOverlapAPipe(pipe[0]) || DoesBirdOverlapAPipe(pipe[1])) {
				this->Die();
			}
		}
	}
}

void FlappyBird::UpdatePhysics(double dt) {
	this->VerticalVelocity += this->GRAVITY;

	const float* lastPosition = this->Bird->GetPosition();
	float newPosition[2] = { 0.0f, (float)dt * (lastPosition[1] + this->VerticalVelocity) };

	this->Bird->Translate(newPosition);

	double newRotation = this->VerticalVelocity / 1e2;
	newRotation = newRotation < -this->LimitAngle ? -this->LimitAngle : newRotation > this->LimitAngle ? this->LimitAngle : newRotation;

	this->Bird->SetRotation(newRotation + PI);
}

bool FlappyBird::DoesBirdOverlapAPipe(Sprite* pipe) {
	// Getting the vertices of both bird and pipe

	float* topLeftPipe = nullptr;
	float* bottomLeftPipe = nullptr;
	float* bottomRightPipe = nullptr;
	float* topRightPipe = nullptr;

	pipe->GetVertices(topLeftPipe, bottomLeftPipe, bottomRightPipe, topRightPipe);

	float* topLeftBird = nullptr;
	float* bottomLeftBird = nullptr;
	float* bottomRightBird = nullptr;
	float* topRightBird = nullptr;

	this->Bird->GetVertices(topLeftBird, bottomLeftBird, bottomRightBird, topRightBird);

	// Calculate min and max (the rotation of the bird can make the player struggle a lot)
	float birdMinX = std::min({ topLeftBird[0], bottomLeftBird[0], bottomRightBird[0], topRightBird[0] });
	float birdMaxX = std::max({ topLeftBird[0], bottomLeftBird[0], bottomRightBird[0], topRightBird[0] });
	float birdMinY = std::min({ topLeftBird[1], bottomLeftBird[1], bottomRightBird[1], topRightBird[1] });
	float birdMaxY = std::max({ topLeftBird[1], bottomLeftBird[1], bottomRightBird[1], topRightBird[1] });

	float pipeMinX = std::min({ topLeftPipe[0], bottomLeftPipe[0], bottomRightPipe[0], topRightPipe[0] });
	float pipeMaxX = std::max({ topLeftPipe[0], bottomLeftPipe[0], bottomRightPipe[0], topRightPipe[0] });
	float pipeMinY = std::min({ topLeftPipe[1], bottomLeftPipe[1], bottomRightPipe[1], topRightPipe[1] });
	float pipeMaxY = std::max({ topLeftPipe[1], bottomLeftPipe[1], bottomRightPipe[1], topRightPipe[1] });

	// AABB overlap check
	bool aabbX = (birdMinX < pipeMaxX) && (birdMaxX > pipeMinX);
	bool aabbY = (birdMinY < pipeMaxY) && (birdMaxY > pipeMinY);

	return aabbX && aabbY;
}

void FlappyBird::Die() {
	if (DEBUGGING) {
		DEBUG_PRINT("Player has died!");
	}

	hasGameFinished = true;
}

void FlappyBird::SetPipesPosition(Sprite* pipe1, Sprite* pipe2, float startX) {
	int gabY = (rand() % 100) - 50;

	float positionPipe2[2] = { startX, gabY - this->gapVerticalSize - pipe1->height };
	float positionPipe1[2] = { startX, gabY + this->gapVerticalSize + pipe2->height };

	pipe1->SetPosition(positionPipe1);
	pipe2->SetPosition(positionPipe2);
}

void FlappyBird::OnInputDetected(int input) {
	if (hasGameFinished) { return; }

	if (input != GLFW_KEY_SPACE) { return; }

	if (!this->hasGameStarted) {
		this->hasGameStarted = true;

		return;
	}

	if (this->VerticalVelocity <= 0.01f) { // Cannot spam the jump button
		this->VerticalVelocity = this->JumpPower;
	}
}

void FlappyBird::OnGameClose() {
	for (std::array<Sprite*, 2> pipe : this->Pipes) {
		delete pipe[0];
		delete pipe[1];
	}
}