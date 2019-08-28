// Emitter code taken and modified from class example
//
// by Kevin M. Smith - CS 134 SJSU
#include "Emitter.h"

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	if (spriteSys == NULL)
		sys = new SpriteSystem();
	else
		sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;

	lastSpawned = 0;
	lives = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec2f(100, 100);
	drawable = true;
	width = 50;
	height = 50;
	force = NULL;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {

	// Draw the emitter's sprite
	if (drawable) {
		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + getX(), -image.getHeight() / 2.0 + getY());
		}
		else {
			//ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + getX(), -height / 2 + getY(), width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {

	sys->update();
	if (!started) return;

	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {
		spawn(time);
	}
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate))
		lastSpawned = ofGetElapsedTimeMillis() - 1000;
}

void Emitter::spawn(float time) {
	// spawn a new sprite
	Sprite sprite;
	// Choose which child image to set
	if (haveChildImage) {
		float val = ofRandom(0, 3);
		if (has2ndChild && val < 1)
			sprite.setImage(childImage2);
		else if (has3rdChild && val < 2)
			sprite.setImage(childImage3);
		else
			sprite.setImage(childImage);
	}
	if (lives != 0) sprite.setLives(lives);
	sprite.setTag(getTag());
	sprite.setVelocity(velocity);
	sprite.setLifespan(lifespan);
	sprite.setPosition(getPosition() - ofVec2f(childImage.getWidth() / 2, 0));
	sprite.setBirthTime(time);
	sys->add(sprite);
	lastSpawned = time;
	if (hasSound)
		fireSound.play();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec2f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::set2ndChildImage(ofImage img) {
	childImage2 = img;
	has2ndChild = true;
}

void Emitter::set3rdChildImage(ofImage img) {
	childImage3 = img;
	has3rdChild = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::setSize(ofVec2f val) {
	width = val.x;
	height = val.y;
}

void Emitter::setSound(string filename) {
	fireSound.load(filename);
	fireSound.setMultiPlay(true);
	hasSound = true;
}

void Emitter::reset() {

	if (sys->sprites.empty()) return;

	lastSpawned = ofGetElapsedTimeMillis();
	started = false;

	vector<Sprite>::iterator s = sys->sprites.begin();
	vector<Sprite>::iterator tmp;

	while (s != sys->sprites.end()) {
		tmp = sys->sprites.erase(s);
		s = tmp;
	}
}