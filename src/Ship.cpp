#include "Ship.h"


//--------------------------------------------------------------
Ship::Ship() : Emitter(new SpriteSystem()) {
	// Set default move state
	_movement = straight;
	// Set default animation counter
	exhaustCounter = 0;
	// Set position of ship and it's emitter
	position = ofVec2f(264, 750);
	setPosition(position);
	// Set emitter velocity and rate
	setVelocity(ofVec2f(0, -750));
	setRate(3.5);
	setSound("sounds/pew.mp3");
	drawable = false;
	// Load additional assets
	loadAssets();
}

//--------------------------------------------------------------
void Ship::loadAssets() {
	sprite.loadImage("sprites/highwind.png");
	sprite.setAnchorPercent(0.5, 0.5);

	exhaustSprite.loadImage("sprites/exhaust.png");
	exhaustSprite.setAnchorPercent(0.5, 0.5);

	// Set projectile sprite
	ofImage img;
	img.load("sprites/lazer.png");
	img.setAnchorPercent(0.5, 0.5);
	setChildImage(img);

}

//--------------------------------------------------------------
void Ship::exhaustAnim() {
	if (exhaustCounter == 19)
		exhaustCounter = -1;
	exhaustCounter++;
}

void Ship::checkMovement() {
	// Update left and right movement based on what's being pressed
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

	// Update up and down movement based on what's being pressed
	if ((holdingUp && holdingDown) || (!holdingUp && !holdingDown))
		velocity.y = 0;
	else if (holdingUp)
		velocity.y = -1;
	else if (holdingDown)
		velocity.y = 1;
}

//--------------------------------------------------------------
void Ship::draw() {

	Emitter::draw();

	ofSetColor(ofColor::white);
	// X position, Y Position, Width, Height, X anchor, Y anchor
	exhaustSprite.drawSubsection(X() + 1, Y() + 70 + (exhaustCounter / 5), 132, 63, 
		132 * _movement, 64 * (exhaustCounter / 5));
	sprite.drawSubsection(position.x, position.y, 132, 134, 131 * _movement, 0);

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
	case 'W':
	case 'w':
	case OF_KEY_UP:
		holdingUp = true;
		break;
	case 'S':
	case 's':
	case OF_KEY_DOWN:
		holdingDown = true;
		break;
	case ' ':
		firing = true;
		start();
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
	case 'W':
	case 'w':
	case OF_KEY_UP:
		holdingUp = false;
		break;
	case 'S':
	case 's':
	case OF_KEY_DOWN:
		holdingDown = false;
		break;
	case ' ':
		firing = false;
		stop();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void Ship::update() {
	
	// Update emitter position to ship position
	switch (_movement) {
	case straight: Emitter::setPosition(position + ofVec2f(0, -50));
		break;
	case left: Emitter::setPosition(position + ofVec2f(-35, -50));
		break;
	case right: Emitter::setPosition(position + ofVec2f(35, -50));
		break;
	default:
		break;
	}

	// If the ship is being moved with the mouse, these won't be used
	if (!clicked)
		checkMovement();

	// Move the ship based on current velocity
	position += speed * velocity;

	// Check left/right bounds
	if (position.x < 20)
		position.x = 20;
	else if (position.x > 508)
		position.x = 508;

	// Check up/down bounds
	if (position.y > 800)
		position.y = 800;
	else if (position.y < 200)
		position.y = 200;

	// Fire lazers if space is being held
	Emitter::update();

	// Animate flames
	exhaustAnim();

}

//--------------------------------------------------------------
void Ship::move(ofVec2f val) {
	cout << val.x << endl;
	if (val.x > 3)
		_movement = right;
	else if (val.x < -3)
		_movement = left;
	else
		_movement = straight;

	position += val;
}

//--------------------------------------------------------------
void Ship::reset() {
	// Set default move state
	_movement = straight;
	// Set default animation counter
	exhaustCounter = 0;
	// Set position of ship and it's emitter
	position = ofVec2f(264, 750);
	setPosition(position);
	// Set emitter velocity and rate
	setVelocity(ofVec2f(0, -750));
	setRate(3.5);
	firing = false;
	holdingLeft = false;
	holdingRight = false;
	holdingUp = false;
	holdingDown = false;
	clicked = false;
	Emitter::reset();
}

//--------------------------------------------------------------
Ship::~Ship() {

}