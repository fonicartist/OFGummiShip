#pragma once

#include "ofMain.h"

class Ship {
	private:
		// Openframeworks variables
		ofImage sprite;
		ofImage exhaustSprite;
		glm::vec2 position;
		glm::vec2 velocity;

		// Regular variables
		enum Movement { left, straight, right } _movement;

		float speed = 10.0f;
		int exhaustCounter;
		bool holdingLeft = false,
			 holdingRight = false;

		// Private methods
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

};