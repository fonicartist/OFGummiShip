#include "Boss.h"

// Boss constructor
Boss::Boss() : Emitter(new SpriteSystem()) {
	// Emitter settings
	active = false;
	attacking = false;
	drawable = false;
	hasLeftArm = true;
	hasRightArm = true;
	canShoot = false;
	canTackle = false;
	started = false;
	shooting = false;
	hitAgain = true;
	physicsParticle.position = ofVec2f(274, -100);
	setVelocity(ofVec2f(0, 550));
	setRate(0.01);
	setLifespan(5000);
	setTag("BossLazer");
	setSound("sounds/boss_fires_lazer2.mp3");
	shootCounter = 0;
	lastShootTime = 0;
	shootCooldownTimer = 0;
	tackleCooldownTimer = 0;
	leftArmLives = 60;
	rightArmLives = 60;
	setLives(150);

	// Image settings
	width = 130 * scale;
	height = 122 * scale;
	sprite.load("sprites/boss_body.png");
	sprite.resize(520 * scale, 244 * scale);
	xFrame = 0;
	yFrame = 0;
	lastAnimTime = ofGetElapsedTimeMillis();

	// Set arm sprites
	armWidth = 81 * scale;
	armHeight = 125 * scale;
	leftArm.load("sprites/boss_left_arms2.png");
	leftArm.resize(armWidth * 2, armHeight * 2);
	rightArm.load("sprites/boss_right_arms2.png");
	rightArm.resize(armWidth * 2, armHeight * 2);
	yArmFrame = 0;
	xArmFrame = 0;
	lastArmAnimTime = lastAnimTime;

	// Set projectile sprite
	ofImage img;
	img.load("sprites/boss_lazer.png");
	img.setAnchorPercent(0.5, 1);
	setChildImage(img);

	// Set Drill Attack Sound
	drill.load("sounds/drill2.mp3");
	drill.setMultiPlay(false);
	drill.setLoop(true);

	// Set Forces that will affect the boss
	turbForce = new TurbulenceForce(ofVec3f(-50,-20, 0), ofVec3f(50, 20, 0));
	gravForce = new GravityForce(ofVec3f(0, 20, 0));
	antiGravForce = new GravityForce(ofVec3f(0, -30, 0));
	impForce = new ImpulseForce(200);
	impForce->setDir(ofVec3f(0, -1, 0));
	tackleForce = new ImpulseForce(40000);
	tackleForce->setDir(ofVec3f(0, 1, 0));

}

void Boss::update(ofVec2f player) {

	// Actions the boss will do once it is set to active
	// This will be when it has reached a certain height
	// after entering from the top of the screen
	if (active) {

		sys->update();

		// Update the particle's position using physics and set
		// the boss to be the same position if not moving for an attack
		updateForces(player);
		physicsParticle.integrate();
		setPosition(physicsParticle.position);

		// Update animations
		updateAnim();

		// The boss will shoot at the player until shootCounter reaches 9
		// After that, it will perform a drill attack that charges the player
		if (!attacking && getY() > 160) {

			// Boss will only tackle if it still has either arms
			if (shootCounter == 9 && (hasLeftArm || hasRightArm))
				attacking = true;

			float time = ofGetElapsedTimeMillis();

			// Boss will pause between every three shots before firing again
			if ((hasLeftArm || hasRightArm) && (time - shootCooldownTimer) > 3500) {
				if ((time - lastShootTime) > 1000) {
					lastShootTime = time;
					shooting = true;
					shootCounter++;
					spawn(time);
				}
				if (shootCounter % 3 == 0)
					shootCooldownTimer = time;
			}

			// Boss does something different once it loses both arms
			if (!(hasLeftArm || hasRightArm) && (time - shootCooldownTimer) > 2750) {
				if ((time - lastShootTime) > 250) {
					lastShootTime = time;
					shooting = true;
					shootCounter++;
					spawn(time);
				}
				if (shootCounter % 3 == 0)
					shootCooldownTimer = time;
			}

		}
		else if (attacking && !drill.isPlaying()) {
			drill.play();
		}
		else if (!attacking && drill.isPlaying())
			drill.stop();

	}

	// Update Boss' lazers
	sys->update();
}

void Boss::updateAnim() {

	// Change animation row if boss is shooting a lazer
	if (shooting)
		yFrame = 1;
	else
		yFrame = 0;

	// Change shoot state to false after some time
	if (shooting && ofGetElapsedTimeMillis() - lastShootTime > 500)
		shooting = false;

	// Change arm animation row and position if boss is tackling
	if (!attacking) {
		yArmFrame = 0;
		leftArmPos = ofVec2f(getX() - 175, getY() - 60);
		rightArmPos = ofVec2f(getX() + 55, getY() - 60);
	}
	else {
		yArmFrame = 1;
		leftArmPos = ofVec2f(getX() - 135, getY() - 15);
		rightArmPos = ofVec2f(getX() + 15, getY() - 15);
	}

	// Move animation frame forward after certain time has passed.
	if (ofGetElapsedTimeMillis() - lastAnimTime > 300) {
		lastAnimTime = ofGetElapsedTimeMillis();
		xFrame++;
	}
	if (ofGetElapsedTimeMillis() - lastArmAnimTime > 100) {
		lastArmAnimTime = ofGetElapsedTimeMillis();
		xArmFrame++;
	}

	// Make sure the animation frame stays within spritesheet
	if (xFrame > 3)
		xFrame = 0;
	if (xArmFrame > 1)
		xArmFrame = 0;

}

void Boss::updateForces(ofVec2f player) {
	// Inch boss towards player's x position
	int direction = (player.x - physicsParticle.position.x) / abs((player.x - physicsParticle.position.x));
	physicsParticle.velocity.x = direction * 30;

	// Boss will do a different action if it is attacking
	// with a tackle. Different forces will be applied.
	if (!attacking) {
		// Update forces
		turbForce->updateForce(&physicsParticle);
		gravForce->updateForce(&physicsParticle);

		// Boss will jet back to top of screen if it drifts too far
		if (getY() > 210)
			impForce->updateForce(&physicsParticle);
	}
	else {
		// Boss floats to the top of screen before tackling
		antiGravForce->updateForce(&physicsParticle);

		// Send the boss flying down the screen
		if (getY() < 120)
			tackleForce->updateForce(&physicsParticle);

		// Reset the boss to above the screen
		if (getY() > 900) {
			physicsParticle.position.y = -150;
			attacking = false;
			hitAgain = true;
			shootCounter = 0;
			physicsParticle.velocity.y = physicsParticle.velocity.y / 8;
		}
	}

	// Check side bounds of the boss and only allow 
	// 1/3 of the boss to be offscreen on the left or right
	if (physicsParticle.position.x < -width * scale / 3)
		physicsParticle.position.x = -width * scale / 3;
	else if (physicsParticle.position.x > ofGetScreenWidth() - 2 * width * scale / 3)
		physicsParticle.position.x = ofGetScreenWidth() - 2 * width * scale / 3;
}

void Boss::draw() {
	ofSetColor(ofColor::white);
	// X position, Y Position, Width, Height, X anchor, Y anchor
	sprite.drawSubsection(getX() - width / 2, getY() - height / 2, width, height, width * xFrame, height *yFrame);
	//Emitter::draw();

	// Draw Boss arms
	float offset = xFrame * 1.2;
	if (!attacking) {
		if (hasLeftArm)
			leftArm.drawSubsection(leftArmPos.x - ofRandom(0, 1), leftArmPos.y - offset, armWidth, armHeight, armWidth * xArmFrame, armHeight *yArmFrame);
		if (hasRightArm)
			rightArm.drawSubsection(rightArmPos.x + ofRandom(0, 1), rightArmPos.y - offset, armWidth, armHeight, armWidth * xArmFrame, armHeight *yArmFrame);
	}
	else {
		if (hasLeftArm)
			leftArm.drawSubsection(leftArmPos.x - ofRandom(0, 1), leftArmPos.y - offset * .5, armWidth, armHeight, armWidth * xArmFrame, armHeight *yArmFrame);
		if (hasRightArm)
			rightArm.drawSubsection(rightArmPos.x + ofRandom(0, 1), rightArmPos.y - offset * .5, armWidth, armHeight, armWidth * xArmFrame, armHeight *yArmFrame);
	}

	// Draw the boss's lazers
	sys->draw();

}

void Boss::reset() {
	Emitter::reset();
	active = false;
	attacking = false;
	drawable = false;
	hasLeftArm = true;
	hasRightArm = true;
	canShoot = false;
	canTackle = false;
	started = false;
	shooting = false;
	hitAgain = true;
	physicsParticle.position = ofVec2f(274, -100);
	shootCounter = 0;
	lastShootTime = 0;
	shootCooldownTimer = 0;
	tackleCooldownTimer = 0;
	leftArmLives = 60;
	rightArmLives = 60;
	setLives(150);
	drill.stop();
}

void Boss::shoot() {
	Emitter::reset();
	start();
}

// Boss Destructor
Boss::~Boss() {

}