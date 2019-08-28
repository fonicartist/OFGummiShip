#pragma once

#include "ofMain.h"
#include "Object2D.h"
#include "Particle.h"
#include "ParticleSystem.h"

class Sprite : public Object2D, public Particle {
	private:

		ofImage sprite;

		TurbulenceForce *tForce;

		bool hasImage, hitAgain, hasForce;

		int lives;

		float birthTime, lifespan;

	public:

		Sprite();
		Sprite(string);
		Sprite(string, float, float);
		Sprite(string, ofVec2f);

		bool getImageStatus() { return hasImage; }
		bool getHitAgain() { return hitAgain; }
		int getLives() { return lives; }
		float getAge() { return ofGetElapsedTimeMillis() - birthTime; }
		float getLifespan() { return lifespan; }
		float getWidth() { if (hasImage) return sprite.getWidth(); else return 0; }
		float getHeight() { if (hasImage) return sprite.getHeight(); else return 0; }
		ofVec2f getCenter() { return ofVec2f(getX() + getWidth() / 2, getY() + getHeight() / 2); }

		void draw();
		void drawSubsection(float, float, float, float);
		void load(string);
		void setAnchorPercent(float, float);
		void setAnchorPoint(float, float);
		void setImage(ofImage img) { sprite = img; hasImage = true; }
		void setLifespan(float val) { lifespan = val; }
		void setBirthTime(float val) { birthTime = val; }
		void setHitAgain(bool val) { hitAgain = val; }
		void setLives(int i) { lives = i; }
		void giveForce() { hasForce = true; }
		void reduceLives() { lives--; }

};

class SpriteSystem {
	public:
		SpriteSystem();
		void add(Sprite);
		void remove(int);
		void update();
		void draw();
		vector<Sprite> sprites;
};