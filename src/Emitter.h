#pragma once
#include "Object2D.h"
#include "Sprite.h"

class Emitter : public Object2D {
	private:

		int lives;
		float fireRate;
		bool hasSound = false, has2ndChild = false, has3rdChild = false;
		ofSoundPlayer fireSound;
		ParticleForce *force;

	public:

		Emitter(SpriteSystem *);
		void draw();
		void start();
		void spawn(float);
		void stop();
		void setLifespan(float);
		void setVelocity(ofVec2f);
		void setChildImage(ofImage);
		void set2ndChildImage(ofImage);
		void set3rdChildImage(ofImage);
		void setImage(ofImage);
		void setRate(float);
		void setSize(ofVec2f);
		void setSound(string);
		void update();
		SpriteSystem *sys;
		ParticleSystem *pSys;
		float rate;
		ofVec2f velocity;
		float lifespan;
		bool started;
		float lastSpawned;
		ofImage childImage, childImage2, childImage3;
		ofImage image;
		bool drawable;
		bool haveChildImage;
		bool haveImage;
		float width, height;
		void reset();
		int getLives() { return lives; }
		void setLives(int i) { lives = i; }
		void reduceLife() { lives--; cout << lives << endl; }
		void setForce(ParticleForce *f) { force = f; }

};