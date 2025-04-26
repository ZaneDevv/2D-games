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
		int gabY = (rand() % 100) - 50;

		float xPosition = (float)this->startingPipesXPosition + i * (float)this->distanceBetweenPipes;

		Sprite* pipe1 = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/pipe-green.png");
		pipe1->SetScale(this->pipesScale);

		Sprite* pipe2 = this->CreateImage("C:/Users/User/source/repos/OpenGL/OpenGL/scr/Games/FlappyBird/Images/pipe-green.png");
		pipe2->Rotate(PI);
		pipe2->SetScale(this->pipesScale);

		float positionPipe2[2] = { xPosition, gabY - this->gapVerticalSize - pipe1->height };
		float positionPipe1[2] = { xPosition, gabY + this->gapVerticalSize + pipe2->height };
		
		pipe1->SetPosition(positionPipe1);
		pipe2->SetPosition(positionPipe2);

		this->Pipes.push_back(pipe1);
		this->Pipes.push_back(pipe2);
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

		for (Sprite* pipe : this->Pipes) {
			if (pipe == nullptr) {
				
				if (DEBUGGING) {
					WARNING_PRINT("Null pipe!");
				}

				continue;
			}

			pipe->Translate(translation);

			if (DoesBirdOverlapAPipe(pipe)) {
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
	// TODO: Rewrite this method

	/*float birdLeft = this->Bird->GetPosition()[0] - this->Bird->width * this->Bird->GetScale();
	float birdRight = this->Bird->GetPosition()[0] + this->Bird->width * this->Bird->GetScale();
	float birdTop = this->Bird->GetPosition()[1] + this->Bird->height * this->Bird->GetScale();
	float birdBottom = this->Bird->GetPosition()[1] - this->Bird->height * this->Bird->GetScale();

	float pipeLeft = pipe->GetPosition()[0] - pipe->width * pipe->GetScale();
	float pipeRight = pipe->GetPosition()[0] + pipe->width * pipe->GetScale();
	float pipeTop = pipe->GetPosition()[1] + pipe->height * pipe->GetScale();
	float pipeBottom = pipe->GetPosition()[1] - pipe->height * pipe->GetScale();

	bool aabbX = birdRight >= pipeLeft && birdLeft <= pipeRight;
	bool aabbY = birdTop >= pipeBottom && birdBottom <= pipeTop;

	return aabbX && aabbY;*/

	return false;
}


void FlappyBird::Die() {
	if (DEBUGGING) {
		DEBUG_PRINT("Player has died!");
	}

	hasGameFinished = true;
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
	for (Sprite* pipe : this->Pipes) {
		delete pipe;
	}
}