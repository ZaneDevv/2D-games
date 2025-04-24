#include "FlappyBird.h"

FlappyBird::FlappyBird() {
	this->screenWidth = 600;
	this->screenHeight = 800;
	this->screenName = "Flappy bird";
}

void FlappyBird::OnScreenCreated() {
	this->CreateImage("C:/Users/User/Pictures/9906ac83676244eb8260e62a902b4d98.jpg");
}

void FlappyBird::Update(double dt) {
	this->UpdatePhysics();
}

void FlappyBird::UpdatePhysics() {

}