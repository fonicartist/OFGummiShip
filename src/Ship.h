#pragma once

#include "ofMain.h"
#include "Emitter.h"

class Ship : public Emitter {
	private:
		// Openframeworks variables
		ofImage sprite;
		ofImage exhaustSprite;
		ofVec2f position;
		ofVec2f velocity;

		// Regular variables
		enum Movement { left, straight, right } _movement;

		float speed = 10.0f;
		int exhaustCounter;
		bool firing = false,
			 holdingLeft = false,
			 holdingRight = false,
			 holdingUp = false,
			 holdingDown = false,
			 clicked = false;

		// Private methods
		void checkMovement();
		void exhaustAnim();
		void loadAssets();

	public:
		// Constructor and Destructor
		Ship();
		~Ship();

		// Common Methods
		void draw();
		void inputDown(int);
		void inputReleased(int);
		void update();
		void move(ofVec2f);
		void reset();

		float X() {
			return position.x;
		}
		float Y() {
			return position.y;
		}
		void setX(float val) {
			position.x = val;
		}
		void setY(float val) {
			position.y = val;
		}
		void changeClick() {
			clicked = !clicked;
		}

};