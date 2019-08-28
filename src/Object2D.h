#pragma once

#include "ofMain.h"

class Object2D {
	private:

		string tag;

		ofVec2f position,
				velocity;

		float angle,
			  speed;

	public:

		// Constructor
		Object2D() {
			setPosition(0, 0);
			setVelocity(0, 0);
			angle = 0;
			speed = 0;
			tag = "";
		}

		// Tag getters and modifiers
		string getTag() { return tag; }
		void setTag(string str) { tag = str; }

		// Position getters and modifiers
		float getX() { return position.x; }
		float getY() { return position.y; }
		ofVec2f getPosition() { return position; }
		void setPosition(float x, float y) { position = ofVec2f(x, y); }
		void setPosition(ofVec2f val) { position = val; }
		void move() { position += velocity; }
		void move(float x, float y) { position += ofVec2f(x, y); }
		void move(ofVec2f val) { position += val; }

		// Speed getters and modifiers
		float getSpeed() { return speed; }
		float setSpeed(float s) { speed = s; }

		// Angle getters and modifiers
		float getAngle() { return angle; }
		void setAngle(float a) { angle = a; }
		void rotate(float a) { angle += a; }

		// Velocity getters and modifiers
		ofVec2f getVelocity() { return velocity; }
		void setVelocity(float x, float y) { velocity = ofVec2f(x, y); }
		void setVelocity(ofVec2f val) { velocity = val; }

};