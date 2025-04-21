#include "FlappyBird.h"

FlappyBird::FlappyBird() {
	this->screenWidth = 600;
	this->screenHeight = 800;
	this->screenName = "Flappy bird";
}

void FlappyBird::OnScreenCreated() {
	
}

void FlappyBird::Update(double dt) {
	this->UpdatePhysics();
	this->UpdateGraphics();
}

void FlappyBird::UpdatePhysics() {

}

void FlappyBird::UpdateGraphics() {

}