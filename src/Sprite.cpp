#include "Sprite.h"

// Creates an empty sprite
Sprite::Sprite() {
	hasImage = false;
	hitAgain = true;
	hasForce = false;
	lifespan = -1;      
	birthTime = 0;
	lives = 0;
}

// Creates a sprite with a given image
Sprite::Sprite(string filename) : Sprite::Sprite() {
	sprite.load(filename);
}

// Creates a sprite with a given image at a position (x, y)
Sprite::Sprite(string filename, float x, float y) : Sprite::Sprite(filename) {
	setPosition(x, y);
}

// Creates a sprite with a given image and position defined by a vector
Sprite::Sprite(string filename, ofVec2f position) : Sprite::Sprite(filename) {
	setPosition(position);
}

// Draws the entire image at its current position
void Sprite::draw() {
	sprite.draw(getPosition());
}

// Draws a subsection of the image with given dimensions from a given anchor point
void Sprite::drawSubsection(float width, float height, float xAnchor, float yAnchor) {
	sprite.drawSubsection(getX(), getY(), width, height, xAnchor, yAnchor);
}

// Load the image file for the sprite with the given file name
void Sprite::load(string filename) {
	sprite.load(filename);
	hasImage = true;
}

// Set the origin point for the image based on percentage
void Sprite::setAnchorPercent(float x, float y) {
	sprite.setAnchorPercent(x, y);
}

// Set the origin point for the image at a given point
void Sprite::setAnchorPoint(float x, float y) {
	sprite.setAnchorPoint(x, y);
}


SpriteSystem::SpriteSystem() {

}

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->getLifespan() != -1 && s->getAge() > s->getLifespan()) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].move(sprites[i].getVelocity() / ofGetFrameRate());
		sprites[i].integrate();
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}