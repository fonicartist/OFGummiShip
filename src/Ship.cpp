#include "Ship.h"


//--------------------------------------------------------------
Ship::Ship() {
	_movement = straight;
	exhaustCounter = 0;
	position = glm::vec2(264, 750);
	loadAssets();
}

//--------------------------------------------------------------
void Ship::loadAssets() {
	sprite.loadImage("sprites/highwind.png");
	sprite.setAnchorPercent(0.5, 0.5);

	exhaustSprite.loadImage("sprites/exhaust.png");
	exhaustSprite.setAnchorPercent(0.5, 0.5);

}

void Ship::exhaustAnim() {
	if (exhaustCounter == 19)
		exhaustCounter = -1;
	exhaustCounter++;
}

//--------------------------------------------------------------
void Ship::draw() {

	// X position, Y Position, Width, Height, X anchor, Y anchor
	exhaustSprite.drawSubsection(X() + 2, Y() + 70 + (exhaustCounter / 5), 132, 63, 
		132 * _movement, 64 * (exhaustCounter / 5));
	sprite.drawSubsection(position.x, position.y, 132, 134, 130 * _movement, 0);
	
}

//--------------------------------------------------------------
void Ship::inputDown(int key) {
	switch (key) {
	case 'A':
	case 'a':
	case OF_KEY_LEFT:
		holdingLeft = true;
		break;
	case 'D':
	case 'd':
	case OF_KEY_RIGHT:
		holdingRight = true;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void Ship::inputReleased(int key) {
	switch (key) {
	case 'A':
	case 'a':
	case OF_KEY_LEFT:
		holdingLeft = false;
		break;
	case 'D':
	case 'd':
	case OF_KEY_RIGHT:
		holdingRight = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void Ship::update() {
	if ((holdingLeft && holdingRight) || (!holdingLeft && !holdingRight)) {
		velocity.x = 0;
		_movement = straight;
	}
	else if (holdingLeft) {
		velocity.x = -1;
		_movement = left;
	}
	else if (holdingRight) {
		velocity.x = 1;
		_movement = right;
	}

	position = position + speed * velocity;
	exhaustAnim();

}

//--------------------------------------------------------------
Ship::~Ship() {

}