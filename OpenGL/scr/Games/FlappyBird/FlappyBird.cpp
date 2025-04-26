#include "FlappyBird.h"

#define PI 3.141592653589793

FlappyBird::FlappyBird() {
	this->screenWidth = 600;
	this->screenHeight = 800;
	this->screenName = "Flappy bird";
}

void FlappyBird::OnScreenCreated() {
	float position[2] = { -60.0f, 0.0f };

	Sprite* sprite = this->CreateImage("C:/Users/User/Pictures/Gorgeous/bm,.jpg");
	sprite->Scale(.05f);
	sprite->Rotate(PI);
	sprite->SetPosition(position);

	Sprite* bird = this->CreateImage("C:/Users/User/Pictures/57c7f2be2f3d86b41c0afe17284d8f71.jpg");
	bird->Scale(.05f);
	bird->Rotate(PI);

	this->Bird = bird;
}

void FlappyBird::Update(double dt) {
	this->UpdatePhysics();
}

void FlappyBird::UpdatePhysics() {
	this->VerticalVelocity += this->GRAVITY;

	const float* lastPosition = this->Bird->GetPosition();
	float newPosition[2] = { lastPosition[0], lastPosition[1] + this->VerticalVelocity };

	this->Bird->Translate(newPosition);
}

void FlappyBird::OnInputDetected(int input) {
	if (input != 32) { return; } // Only space required
	
	this->VerticalVelocity = this->JumpPower;
}