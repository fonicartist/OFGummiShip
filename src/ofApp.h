#pragma once

#include "ofMain.h"
#include "Ship.h"
#include "Boss.h"
#include "Sprite.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

class ofApp : public ofBaseApp{
	private:
		enum gameState {title, inGame, paused, winScreen, gameover, gameReset} _gameState;
		enum gameStage {stage1, stage2, stage3, stage4, stage5, stage6} _gameStage;

		bool mouseClicked = false,
			 win = false;
		float timeOfDeath, timeOfWin;
		int lastKey = -999;
		int score, startTime, lives;

		Ship *GummiShip;

		// Enemy 1 Emitters
		Emitter *bug1,
			    *bug2,
				*bug3,
				*bug4;
		vector<Emitter*> enemy1s;

		// Enemy 2 Emitters
		Emitter *cannon1,
				*cannon2;
		vector<Emitter*> enemy2s;

		// Enemy 3 Emitters
		Emitter *stingray1, 
				*stingray2, 
				*stingray3, 
				*stingray4;
		vector<Emitter*> enemy3sleft, enemy3sright;

		// Boss Enemy
		Boss *Heartless;

		// Explosion Particle Emitter
		ParticleEmitter explosionEmitter, explosionEmitter2;
		ImpulseRadialForce *radialForce;
		TurbulenceForce *turbulenceForce;
		GravityForce *gravityForce;

		// Space effects emitters;
		Emitter *spaceClouds,
				*starDust;

		// Colors for various things
		ofColor collideColor,
				explodeColor1, 
				explodeColor2, 
				fadingColor;

		ofSoundPlayer titleMusic,
					  music1,
					  music2,
					  music3,
					  whiteNoise,
					  crashNoise,
			          hitNoise, 
					  hitNoise2,
					  explodeNoise;

		vector<Sprite> backgrounds;
		vector<ofSoundPlayer> bgmusic;

		ofTrueTypeFont khSystemLarge, 
					   khSystemMedium, 
					   khSystemSmall;

		TurbulenceForce *tForce;
		ImpulseForce *iForce;

		ofVec3f tMin, tMax;

		glm::vec2 lastMouse;
		void loadVbo();
		void loadAssets();
		void loadEnemies();
		void playMusic();

		void checkCollision(Emitter*);
		void checkBossCollision();
		void cleanSlateProtocol();
		void emitDamage();
		void bossExplode();
		void emitRed();
		void emitYellow();
		void emitWhite();
		void emitBlue();

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// textures
		//
		ofTexture  particleTex;

		// shaders
		//
		ofVbo vbo;
		ofShader shader;
		
};
