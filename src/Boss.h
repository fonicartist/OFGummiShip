#include "Emitter.h"

class Boss : public Emitter {
	private:
		// Variables
		bool active, 
			 attacking,
			 shooting,
			 canShoot,
			 canTackle,
			 hasLeftArm,
			 hasRightArm,
			 hitAgain;
		int animCounter,
			shootCounter,
			xFrame, 
			yFrame,
			xArmFrame,
			yArmFrame,
			armWidth,
			armHeight,
			leftArmLives, rightArmLives;
		float lastAnimTime,
			  lastArmAnimTime, 
			  lastShootTime,
			  shootCooldownTimer,
			  tackleCooldownTimer,
			  scale = 1.5;
		string tag;


		// Physics
		Particle physicsParticle;
		TurbulenceForce *turbForce;
		GravityForce *gravForce;
		GravityForce *antiGravForce;
		ImpulseForce *impForce;
		ImpulseForce *tackleForce;

		// Openframeworks Variables
		ofImage sprite, leftArm, rightArm;
		ofSoundPlayer drill;
		//ofVec2f position;

		void updateForces(ofVec2f);

	public:
		Boss();
		~Boss();

		int getLeftLives() { return leftArmLives; }
		int getRightLives() { return rightArmLives; }
		int getArmHeight() { return armHeight; }
		int getArmWidth() { return armWidth; }

		void activate() { active = true; }
		void update(ofVec2f);
		void updateAnim();
		void draw();
		void reset();
		void reduceLeftLives() { leftArmLives--; cout << leftArmLives << endl; }
		void reduceRightLives() { rightArmLives--; cout << rightArmLives << endl; }
		void loseLeft() { hasLeftArm = false; }
		void loseRight() { hasRightArm = false; }
		void shoot();
		void setHitAgain(bool val) { hitAgain = val; }

		bool isActivated() { return active; }
		bool hasLeft() { return hasLeftArm; }
		bool hasRight() { return hasRightArm; }
		bool getHitAgain() { return hitAgain; }
		bool isAttacking() { return attacking; }

		ofVec2f leftArmPos, rightArmPos;
		
};