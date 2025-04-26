#include "FlappyBird.h"

FlappyBird::FlappyBird() {
	this->screenWidth = 600;
	this->screenHeight = 800;
	this->screenName = "Flappy bird";
}

void FlappyBird::OnScreenCreated() {
	this->CreateImage("C:/Users/User/Pictures/Gorgeous/bm,.jpg");
	//sprite->Rotate(2);
}

void FlappyBird::Update(double dt) {
	this->UpdatePhysics();
}

void FlappyBird::UpdatePhysics() {

}