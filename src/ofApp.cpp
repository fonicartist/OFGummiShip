#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	_gameState = title;
	_gameStage = stage1;
	GummiShip = new Ship();
	fadingColor.set(ofColor::white);
	collideColor.set(ofColor(255, 238, 234));
	explodeColor2.set(ofColor(255, 232, 125));
	explodeColor1.set(ofColor::orangeRed);
	score = 0;
	lives = 5;
	startTime = 0;
	timeOfDeath = 0;
	timeOfWin = 0;
	loadAssets();
	loadEnemies();

	// texture loading
	//
	ofDisableArbTex();     // disable rectangular textures

	// load textures
	//
	if (!ofLoadImage(particleTex, "sprites/spark.png")) {
		cout << "Particle Texture File: sprites/spark.png not found" << endl;
		ofExit();
	}

	// load the shader
	//
#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif

}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
	if (explosionEmitter.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < explosionEmitter.sys->particles.size(); i++) {
		points.push_back(explosionEmitter.sys->particles[i].position);
		sizes.push_back(ofVec3f(20));
	}
	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::loadAssets() {
	// Load music files
	titleMusic.load("music/preciousStarsInTheSky.ogg");
	titleMusic.setLoop(true);

	// BG music
	music1.load("music/blastaway1.ogg");
	music1.setLoop(true);
	music2.load("music/blastaway2.ogg");
	music2.setLoop(true);
	music3.load("music/blastaway3.ogg");
	music3.setLoop(true);
	bgmusic.push_back(music1);
	bgmusic.push_back(music2);
	bgmusic.push_back(music3);

	// Sound Effects
	whiteNoise.load("sounds/whitenoise.ogg");
	whiteNoise.setLoop(true);
	crashNoise.load("sounds/crash.ogg");
	crashNoise.setMultiPlay(true);
	hitNoise.load("sounds/lazer_hit2.mp3");
	crashNoise.setMultiPlay(true);
	hitNoise2.load("sounds/lazer_hits_boss2.mp3");
	crashNoise.setMultiPlay(true);
	explodeNoise.load("sounds/enemy_explodes2.mp3");
	crashNoise.setMultiPlay(true);

	// Load fonts
	ofTrueTypeFont::setGlobalDpi(72);
	khSystemLarge.load("fonts/KHGummiShip.ttf", 24, true, true);
	khSystemLarge.setLineHeight(38.0f);
	khSystemLarge.setLetterSpacing(1.037);

	khSystemMedium.load("fonts/KHGummiShip.ttf", 20, true, true);
	khSystemMedium.setLineHeight(28.0f);
	khSystemMedium.setLetterSpacing(1.037);

	khSystemSmall.load("fonts/KHGummiShip.ttf", 12, true, true);
	khSystemSmall.setLineHeight(18.0f);
	khSystemSmall.setLetterSpacing(1.037);


	// Load Background Image
	backgrounds.push_back(Sprite("bg/easybg.png", 264, -8640));
	backgrounds.push_back(Sprite("bg/easybg.png", 264, -17280));
	backgrounds[0].setAnchorPoint(264, -864);
	//backgrounds[0].setPosition(264, -8640);
	backgrounds[1].setAnchorPoint(264, -864);
	//backgrounds[1].setPosition(264, -17280);

	// Set up particle emitter
	radialForce = new ImpulseRadialForce(15);
	turbulenceForce = new TurbulenceForce(ofVec3f(-20, -20, 0), ofVec3f(20, 20, 0));
	gravityForce = new GravityForce(ofVec3f(0, -20, 0));

	explosionEmitter.sys->addForce(radialForce);
	explosionEmitter.sys->addForce(turbulenceForce);
	explosionEmitter.sys->addForce(gravityForce);

	explosionEmitter.setEmitterType(RadialEmitter);
	explosionEmitter.setOneShot(true);
	explosionEmitter.setRandomLife(true);
	explosionEmitter.setLifespanRange(ofVec2f(0.5, 1));
	explosionEmitter.setPosition(ofVec2f(ofGetWindowWidth() / 2, -50));
	explosionEmitter.setVelocity(ofVec3f(120, 0, 0));
	explosionEmitter.setGroupSize(20);
	explosionEmitter.setParticleRadius(5);
	explosionEmitter.particleColor = explodeColor1;

	explosionEmitter2.sys->addForce(radialForce);
	explosionEmitter2.sys->addForce(turbulenceForce);
	explosionEmitter2.sys->addForce(gravityForce);

	explosionEmitter2.setEmitterType(RadialEmitter);
	explosionEmitter2.setOneShot(true);
	explosionEmitter2.setRandomLife(true);
	explosionEmitter2.setLifespanRange(ofVec2f(0.5, 1));
	explosionEmitter2.setPosition(ofVec2f(ofGetWindowWidth() / 2, -50));
	explosionEmitter2.setVelocity(ofVec3f(100, 0, 0));
	explosionEmitter2.setGroupSize(16);
	explosionEmitter2.setParticleRadius(6);
	explosionEmitter2.particleColor = explodeColor2;

	// Space effects emitters
	ofImage img;

	img.load("sprites/cloud1.png");
	img.resize(450, 480);
	spaceClouds = new Emitter(new SpriteSystem());
	spaceClouds->drawable = false;
	spaceClouds->setRate(4);
	spaceClouds->setLifespan(2000);
	spaceClouds->setPosition(ofVec2f(0, 0));
	spaceClouds->setVelocity(ofVec2f(0, 1100));
	spaceClouds->setChildImage(img);
	img.load("sprites/cloud2.png");
	img.resize(430, 500);
	spaceClouds->set2ndChildImage(img);
	img.load("sprites/cloud3.png");
	img.resize(410, 470);
	spaceClouds->set3rdChildImage(img);

	img.load("sprites/stardust.png");
	img.resize(12, 20);
	starDust = new Emitter(new SpriteSystem());
	starDust->drawable = false;
	starDust->setRate(5);
	starDust->setLifespan(1500);
	starDust->setPosition(ofVec2f(0, 0));
	starDust->setVelocity(ofVec2f(0, 1800));
	starDust->setChildImage(img);

}

//--------------------------------------------------------------
void ofApp::loadEnemies() {
	// Enemy type 1
	ofImage img;
	img.load("sprites/enemy01.png");
	img.setAnchorPercent(0.5, 0.5);

	bug1 = new Emitter(new SpriteSystem());
	bug1->setPosition(ofVec2f(70, -70));
	bug2 = new Emitter(new SpriteSystem());
	bug2->setPosition(ofVec2f(200, -70));
	bug3 = new Emitter(new SpriteSystem());
	bug3->setPosition(ofVec2f(330, -70));
	bug4 = new Emitter(new SpriteSystem());
	bug4->setPosition(ofVec2f(460, -70));

	enemy1s.push_back(bug1);
	enemy1s.push_back(bug2);
	enemy1s.push_back(bug3);
	enemy1s.push_back(bug4);

	for (Emitter *m : enemy1s) {
		m->setRate(.2);
		m->setChildImage(img);
		m->setLifespan(5000);
		m->setVelocity(ofVec2f(0, 200));
		m->setTag("Bug");
	}

	// Enemy type 2
	img.load("sprites/enemy02.png");
	img.setAnchorPercent(0.5, 0.5);

	cannon1 = new Emitter(new SpriteSystem());
	cannon1->setPosition(ofVec2f(100, -70));
	cannon2 = new Emitter(new SpriteSystem());
	cannon2->setPosition(ofVec2f(400, -100));

	enemy2s.push_back(cannon1);
	enemy2s.push_back(cannon2);

	// Initialize Turbulence Force to be applied to cannon enemies
	tMin = ofVec3f(-100, -10, 0);
	tMax = ofVec3f(100, 10, 0);
	tForce = new TurbulenceForce(tMin, tMax);

	for (Emitter *m : enemy2s) {
		m->setLives(1);
		m->setRate(.1);
		m->setChildImage(img);
		m->setLifespan(5000);
		m->setVelocity(ofVec2f(0, 200));
		m->setTag("Cannon");
		m->setForce(tForce);
	}

	// Enemy type 3
	stingray1 = new Emitter(new SpriteSystem());
	stingray1->setPosition(-150, 200);
	stingray2 = new Emitter(new SpriteSystem());
	stingray2->setPosition(-277, 136);
	enemy3sleft.push_back(stingray1);
	enemy3sleft.push_back(stingray2);

	stingray3 = new Emitter(new SpriteSystem());
	stingray3->setPosition(678, 200);
	stingray4 = new Emitter(new SpriteSystem());
	stingray4->setPosition(805, 136);
	enemy3sright.push_back(stingray3);
	enemy3sright.push_back(stingray4);

	img.load("sprites/enemy03_left.png");
	for (Emitter *m : enemy3sleft) {
		m->drawable = false;
		m->setLives(2);
		m->setRate(.1);
		m->setChildImage(img);
		m->setLifespan(7000);
		m->setVelocity(ofVec2f(-150, 70));
		m->setTag("Stingray");
	}

	img.load("sprites/enemy03_right.png");
	for (Emitter *m : enemy3sright) {
		m->drawable = false;
		m->setLives(2);
		m->setRate(.1);
		m->setChildImage(img);
		m->setLifespan(7000);
		m->setVelocity(ofVec2f(150, 70));
		m->setTag("Stingray");
	}

	// Boss Enemy
	Heartless = new Boss();
}

//--------------------------------------------------------------
void ofApp::playMusic() {
	switch (_gameState) {
	// Set title music to playing and other bg's to stop if not already
	case title:
		if (!titleMusic.isPlaying()) {
			titleMusic.play();
			for (ofSoundPlayer m : bgmusic)
				if (m.isPlaying())
					m.stop();
		}
		break;
	case inGame:
		if (score < 6000) {
			if (!bgmusic[0].isPlaying()) {
				bgmusic[0].play();
				if (bgmusic[1].isPlaying())
					bgmusic[1].stop();
				if (bgmusic[2].isPlaying())
					bgmusic[2].stop();
				if (titleMusic.isPlaying())
					titleMusic.stop();
			}
		}
		else if (score < 15000) {
			if (!bgmusic[1].isPlaying()) {
				bgmusic[1].play();
				if (bgmusic[0].isPlaying())
					bgmusic[0].stop();
				if (bgmusic[2].isPlaying())
					bgmusic[2].stop();
				if (titleMusic.isPlaying())
					titleMusic.stop();
			}
		}
		else {
			if (!bgmusic[2].isPlaying()) {
				bgmusic[2].play();
				if (bgmusic[0].isPlaying())
					bgmusic[0].stop();
				if (bgmusic[1].isPlaying())
					bgmusic[1].stop();
				if (titleMusic.isPlaying())
					titleMusic.stop();
			}
		}
		break;
	}

	// Play whitenoise when inGame
	if (_gameState == inGame && !whiteNoise.isPlaying())
		whiteNoise.play();
	else if (_gameState != inGame && whiteNoise.isPlaying())
		whiteNoise.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
	switch (_gameState) {
		// ---------------------THIS HAPPENS DURING THE TITLE SEQUENCE
	case title:

		if (fadingColor.a > 50)
			fadingColor.a -= 5;
		else
			fadingColor.a = 255;

		break;
		// ---------------------THIS HAPPENS DURING THE GAME
	case inGame:

		// Check win condition
		if (Heartless->getLives() <= 0) {
			win = true;
			if (timeOfWin == 0) {
				timeOfWin = ofGetElapsedTimeMillis();
				for (Emitter *m : enemy1s)
					m->stop();
				for (Emitter *m : enemy2s)
					m->stop();
				for (Emitter *m : enemy3sleft)
					m->stop();
				for (Emitter *m : enemy3sright)
					m->stop();
			}
		}

		// Let the game play out a bit before moving on to win screen
		if (win && ofGetElapsedTimeMillis() - timeOfWin > 3000) {
			_gameState = winScreen;
		}

		// Destroy remaining enemies after winning
		if (win)
			cleanSlateProtocol();

		// Update the ship
		if (lives > 0)
			GummiShip->update();

		// Update enemy emitters
		for (Emitter *m : enemy1s)
			m->update();
		// Add a new enemy type when score changes
		if (score > 1000 && !win) {
			// Start second spawner if not already after a certain threshold
			if (!cannon2->started && score > 3000)
				cannon2->start();
			for (Emitter *m : enemy2s) {
				m->setVelocity(ofVec2f((GummiShip->getX() - m->getX()) * .1, m->velocity.y));
				m->update();
				// Change emitter's position to a random spot
				m->setPosition(ofVec2f(ofRandom(70, 460), ofRandom(-200, -70)));
			}
		}
		// Add another enemy type when score changes
		if (score > 9000 && !win) {
			//--Left facing stingrays--
			// Change emitter's position to a random height
			stingray1->setPosition(ofVec2f(ofRandom(648, 848), ofRandom(-200, 450)));
			stingray2->setPosition(ofVec2f(stingray1->getPosition()) + ofVec2f(127, -64));
			stingray1->update();
			stingray2->update();

			//--Right facing stingrays--
			// Change emitter's position to a random height
			stingray3->setPosition(ofVec2f(ofRandom(-320, -120), ofRandom(-180, 430)));
			stingray4->setPosition(ofVec2f(stingray3->getPosition()) + ofVec2f(-127, -64));
			stingray3->update();
			stingray4->update();

		}
		// Add boss battle after 15,000 pts
		if (score > 15000 && !win) {
			if (!Heartless->isActivated())
				Heartless->activate();
			Heartless->update(GummiShip->getPosition());
		}

		// Update and move backgrounds
		for (int i = 0; i < backgrounds.size(); i++) {
			backgrounds[i].move(0, 24);
			if (backgrounds[i].getY() >= 0)
				backgrounds[i].setPosition(264, -17280);
		}

		// Add score based on time passed
		if ((ofGetSystemTimeMillis() - startTime) / 50 > 1) {
			score += 1;
			startTime = ofGetSystemTimeMillis();
		}

		// Check for collisions between different objects
		if (lives > 0 && !win) {
			for (Emitter *m : enemy1s)
				checkCollision(m);
			for (Emitter *m : enemy2s)
				checkCollision(m);
			for (Emitter *m : enemy3sleft)
				checkCollision(m);
			for (Emitter *m : enemy3sright)
				checkCollision(m);
			if (Heartless->isActivated())
				checkBossCollision();
		}

		// Update particle emitters
		explosionEmitter.update();
		explosionEmitter2.update();

		// Update space effects emitters
		spaceClouds->setPosition(ofRandomWidth(), -400 - ofRandomHeight() / 4);
		spaceClouds->update();
		starDust->setPosition(ofRandomWidth(), ofRandomHeight() / 4 - 250);
		starDust->update();

		// Change enemy and player stats based on score
		if (score > 500 && _gameStage == stage1) {
			_gameStage = stage2;
			for (Emitter *m : enemy1s) {
				m->setRate(.3);
				m->setLives(0);
			}
			for (Emitter *m : enemy2s)
				m->setLives(1);
			spaceClouds->setRate(4);
			starDust->setRate(5);
			
		}
		if (score > 3000 && _gameStage == stage2) {
			_gameStage = stage3;
			// GummiShip gets a weapon upgrade
			GummiShip->setRate(4.0);
			starDust->setRate(5.5);
		}
		if (score > 6000 && _gameStage == stage3){
			_gameStage = stage4;
			// GummiShip gets a weapon upgrade
			GummiShip->setRate(4.6);
			// Update enemy rates
			for (Emitter *m : enemy1s)
				m->setRate(.38);
			for (Emitter *m : enemy2s) {
				m->setRate(.35);
				m->setLives(2);
			}
			spaceClouds->setRate(4.2);
			starDust->setRate(6.0);
		}
		if (score > 15000 && _gameStage == stage4) {
			_gameStage = stage5;
			// GummiShip gets a weapon upgrade
			GummiShip->setRate(5.3);
			// Update enemy rates
			for (Emitter *m : enemy1s) {
				m->setRate(.17);
				m->setLives(1);
			}
			for (Emitter *m : enemy2s)
				m->setRate(.25);
			for (Emitter *m : enemy3sleft)
				m->setRate(.075);
			for (Emitter *m : enemy3sright)
				m->setRate(.07);
			spaceClouds->setRate(5);
			starDust->setRate(6.5);
		}
		if (score > 25000 && _gameStage == stage5) {
			_gameStage = stage6;
			// GummiShip gets a weapon upgrade
			GummiShip->setRate(5.5);
			// Update enemy rates
			for (Emitter *m : enemy1s) 
				m->setRate(.1);
			for (Emitter *m : enemy2s)
				m->setRate(.16);
			for (Emitter *m : enemy3sleft)
				m->setRate(.055);
			for (Emitter *m : enemy3sright)
				m->setRate(.049);
		}

		if (lives <= 0) {
			if (timeOfDeath == 0)
				timeOfDeath = ofGetElapsedTimeMillis();

			if (ofGetElapsedTimeMillis() - timeOfDeath > 1000)
				_gameState = gameover;
		}

		break;

		// ---------------------THIS HAPPENS WHEN THE GAME IS PAUSED
	case paused:
		if (fadingColor.a > 50)
			fadingColor.a -= 5;
		else
			fadingColor.a = 255;
		break;

		// ---------------------THIS HAPPENS WHEN THE GAME IS WON
	case winScreen:

		// Update Ship
		GummiShip->update();

		// Update and move backgrounds
		for (int i = 0; i < backgrounds.size(); i++) {
			backgrounds[i].move(0, 24);
			if (backgrounds[i].getY() >= 0)
				backgrounds[i].setPosition(264, -17280);
		}

		// Update particle emitters
		explosionEmitter.update();
		explosionEmitter2.update();

		// Update space effects emitters
		spaceClouds->setPosition(ofRandomWidth(), -400 - ofRandomHeight() / 4);
		spaceClouds->update();
		starDust->setPosition(ofRandomWidth(), ofRandomHeight() / 4 - 250);
		starDust->update();

		break;
		// ---------------------THIS HAPPENS WHEN THE GAME IS OVER
	case gameover:
		
		break;
	case gameReset:
		lives = 5;
		win = false;
		timeOfDeath = 0;
		timeOfWin = 0;
		GummiShip->reset();
		for (Emitter *m : enemy1s)
			m->reset();
		for (Emitter *m : enemy2s)
			m->reset();
		for (Emitter *m : enemy3sleft)
			m->reset();
		for (Emitter *m : enemy3sright)
			m->reset();
		spaceClouds->reset();
		starDust->reset();
		Heartless->reset();
		backgrounds[0].setPosition(264, -8640);
		backgrounds[1].setPosition(264, -17280);
		_gameState = title;
		break;

	default:
		break;
	}
	playMusic();
}

//--------------------------------------------------------------
void ofApp::draw(){
	switch (_gameState) {
	// 
	case title:
		ofSetColor(ofColor::white);
		backgrounds[0].draw();
		GummiShip->draw();
		ofSetColor(fadingColor);
		khSystemLarge.drawString("  PRESS\n  SPACE \nTO START", 120, 300);
		break;
	// 
	case inGame:
	case paused:
		// Change background color based on score/difficulty
		if (score < 3000)
			ofSetColor(ofColor::white);
		else if (score < 6000)
			ofSetColor(ofColor(240, 220, 240));
		else if (score < 15000)
			ofSetColor(ofColor(250, 150, 220));
		else 
			ofSetColor(ofColor(240, 90, 180));
		for (int i = 0; i < backgrounds.size(); i++)
			backgrounds[i].draw();

		// Draw effects
		if (score < 3000)
			ofSetColor(ofColor(28, 240, 166, 50));
		else if (score < 6000)
			ofSetColor(ofColor(155, 213, 50, 50));
		else if (score < 15000)
			ofSetColor(ofColor(225, 60, 99, 100));
		else
			ofSetColor(ofColor(214, 3, 151, 100));
		spaceClouds->sys->draw();
		ofSetColor(ofColor::white);
		starDust->sys->draw();
		
		// Draw ship
		ofSetColor(ofColor::white);
		if (lives > 0)
			GummiShip->draw();

		// Draw enemies
		for (Emitter *m : enemy1s)
			m->draw();
		for (Emitter *m : enemy2s)
			m->draw();
		for (Emitter *m : enemy3sleft)
			m->draw();
		for (Emitter *m : enemy3sright)
			m->draw();
		if (Heartless->isActivated() && Heartless->getLives() > 0)
			Heartless->draw();

		// Draw explosions
		explosionEmitter.sys->draw();
		explosionEmitter2.sys->draw();

		/*
		loadVbo();

		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofEnablePointSprites();
		shader.begin();

		//particleTex.bind();
		//ofSetColor(explosionEmitter.particleColor);
		//vbo.draw(GL_POINTS, 0, (int)explosionEmitter.sys->particles.size());
		//particleTex.unbind();

		shader.end();
		ofDisablePointSprites();
		ofDisableBlendMode();
		ofEnableAlphaBlending();
		*/
		
		// Draw score
		ofSetColor(ofColor::black);
		khSystemSmall.drawString("SCORE", 13, 33);
		ofSetColor(ofColor::white);
		khSystemSmall.drawString("SCORE", 10, 30);
		ofSetColor(ofColor::black);
		khSystemMedium.drawString(ofToString(score), 13, 66);
		ofSetColor(ofColor(243, 191, 33));
		khSystemMedium.drawString(ofToString(score), 10, 63);

		// Draw Lives
		ofSetColor(ofColor::black);
		khSystemSmall.drawString("LIVES", 433, 33);
		ofSetColor(ofColor::white);
		khSystemSmall.drawString("LIVES", 430, 30);
		ofSetColor(ofColor::black);
		khSystemMedium.drawString(ofToString(lives), 483, 66);
		ofSetColor(ofColor(243, 191, 33));
		khSystemMedium.drawString(ofToString(lives), 480, 63);

		// Draw 'PAUSED' text
		if (_gameState == paused) {
			ofSetColor(fadingColor);
			khSystemLarge.drawString(" PAUSED", 120, 300);
		}

		break;
	case winScreen:
		// Change background color based on score/difficulty
		if (score < 3000)
			ofSetColor(ofColor::white);
		else if (score < 6000)
			ofSetColor(ofColor(240, 220, 240));
		else if (score < 15000)
			ofSetColor(ofColor(250, 150, 220));
		else
			ofSetColor(ofColor(240, 90, 180));
		for (int i = 0; i < backgrounds.size(); i++)
			backgrounds[i].draw();

		// Draw effects
		if (score < 3000)
			ofSetColor(ofColor(28, 240, 166, 50));
		else if (score < 6000)
			ofSetColor(ofColor(155, 213, 50, 50));
		else if (score < 15000)
			ofSetColor(ofColor(225, 60, 99, 100));
		else
			ofSetColor(ofColor(214, 3, 151, 100));
		spaceClouds->sys->draw();
		ofSetColor(ofColor::white);
		starDust->sys->draw();

		// Draw ship
		ofSetColor(ofColor::white);
		GummiShip->draw();

		// Draw ParticleEmitters
		explosionEmitter.draw();
		explosionEmitter2.draw();

		// Draw on screen Text
		ofSetColor(ofColor::black);
		khSystemLarge.drawString("   YOU\n   WIN!", 138, 323);
		khSystemSmall.drawString("SCORE", 13, 33);
		khSystemMedium.drawString(ofToString(score), 13, 66);
		khSystemMedium.drawString(ofToString(lives), 483, 66);
		ofSetColor(ofColor::white);
		khSystemLarge.drawString("   YOU\n   WIN!", 135, 320);
		khSystemSmall.drawString("SCORE", 10, 30);
		khSystemSmall.drawString("LIVES", 430, 30);
		ofSetColor(ofColor(243, 191, 33));
		khSystemMedium.drawString(ofToString(score), 10, 63);
		khSystemMedium.drawString(ofToString(lives), 480, 63);
	
		break;
	case gameover:
		ofSetColor(ofColor::white);
		khSystemLarge.drawString("  GAME\n  OVER", 140, 320);
		khSystemSmall.drawString("SCORE", 10, 30);
		khSystemMedium.drawString(ofToString(score), 10, 63);
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	// Check if last key pressed is the same as the current
	if (lastKey == key)
		return;
	else
		lastKey = key;

	switch (_gameState) {
	// 
	case title:
		if (key == ' ') {
			_gameState = inGame;
			_gameStage = stage1;
			score = 0;
			startTime = ofGetSystemTimeMillis();
			ofSetColor(ofColor::white);

			// Start enemy emitters
			for (Emitter *m : enemy1s)
				m->start();
			cannon1->start();
			for (Emitter *m : enemy3sleft)
				m->start();
			for (Emitter *m : enemy3sright)
				m->start();
			
			// Start effect emitters
			spaceClouds->start();
			starDust->start();
		}
		break;
	//
	case inGame:
		GummiShip->inputDown(key);
		if (key == 'p' || key == 'P')
			_gameState = paused;
		break;
	case paused:
		// Pressing any key during pause state will resume game
		_gameState = inGame;
		break;
	case winScreen:
	case gameover:
		if (key == ' ') {
			_gameState = gameReset;
		}
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (lastKey == key)
		lastKey = -999;

	switch (_gameState) {
	// 
	case inGame:
	case winScreen:
		GummiShip->inputReleased(key);
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (_gameState == inGame) {
		if (mouseClicked)
			GummiShip->move(glm::vec2(x, y) - lastMouse);

		// Store new mouse position
		lastMouse = glm::vec2(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (_gameState == inGame) {
		// Store mouse position
		lastMouse = glm::vec2(x, y);
		mouseClicked = true;
		GummiShip->changeClick();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (_gameState == inGame) {
		mouseClicked = false;
		GummiShip->changeClick();
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::checkCollision(Emitter *emitter) {

	// Check collisions between lazers and enemies
	for (vector<Sprite>::iterator lazerIter = GummiShip->sys->sprites.begin(); lazerIter != GummiShip->sys->sprites.end(); lazerIter++) {
		
		// Cycle through the emitter's sprites
		for (vector<Sprite>::iterator enemyIter = emitter->sys->sprites.begin(); enemyIter != emitter->sys->sprites.end(); enemyIter++) {

			// Distance between the two sprites
			float distance = lazerIter->getCenter().distance(enemyIter->getCenter());
			float height = enemyIter->getHeight();
			float width = enemyIter->getWidth();
			float contact = height / 2 + lazerIter->getHeight() / 2 - 20;

			// If they collide delete the lazer, add points to the score and reduce enemy health
			if (distance < contact) {
				lazerIter->setLifespan(0);
				explosionEmitter.setPosition(enemyIter->getPosition() + ofVec2f(width/2, height/2));
				explosionEmitter2.setPosition(explosionEmitter.getPosition());

				string str = enemyIter->getTag();
				if (!str.compare("Bug")) {
					if (enemyIter->getLives() == 0) {
						enemyIter->setLifespan(0);
						score += 75;
						hitNoise.play();
						explodeNoise.play();
						emitYellow();
						emitRed();
					}
					else {
						hitNoise.play();
						enemyIter->reduceLives();
						score += 20;
						emitWhite();
						emitBlue();
					}
				}
				else if (!str.compare("Cannon")) {
					if (enemyIter->getLives() == 0) {
						enemyIter->setLifespan(0);
						score += 150;
						hitNoise.play();
						explodeNoise.play();
						emitYellow();
						emitRed();
					}
					else {
						hitNoise.play();
						enemyIter->reduceLives();
						score += 30;
						emitWhite();
						emitBlue();
					}
				}
				else if (!str.compare("Stingray")) {
					if (enemyIter->getLives() == 0) {
						enemyIter->setLifespan(0);
						score += 250;
						hitNoise.play();
						explodeNoise.play();
						emitYellow();
						emitRed();
					}
					else {
						hitNoise.play();
						enemyIter->reduceLives();
						score += 35;
						emitWhite();
						emitBlue();
					}
				}
			}

		}

	}

	// Check if enemy collides with player ship
	// Colliding with a ship automatically destroys it
	// but at the cost of one of the player's life and score
	for (vector<Sprite>::iterator enemyIter = emitter->sys->sprites.begin(); enemyIter != emitter->sys->sprites.end(); enemyIter++) {
		float distance = (GummiShip->getPosition() + ofVec2f(0, 40)).distance(enemyIter->getCenter());
		float contact = enemyIter->getHeight() / 2 + 114 / 2;

		if (distance < contact && enemyIter->getHitAgain()) {
			crashNoise.play();
			enemyIter->setHitAgain(false);
			enemyIter->setLifespan(0);
			score -= 50;
			lives--;
			explosionEmitter.setPosition((GummiShip->getPosition() + enemyIter->getCenter()) / 2);
			explosionEmitter2.setPosition(explosionEmitter.getPosition());
			emitDamage();
		}

	}
}

// Check if player is hit by the boss's lazers
// Or the boss's body when tackled
// Or if the player's lazer hits the boss
void ofApp::checkBossCollision() {

	// Iterate through the enemy lazers
	for (vector<Sprite>::iterator lazerIter = Heartless->sys->sprites.begin(); lazerIter != Heartless->sys->sprites.end(); lazerIter++) {
		float distance = (GummiShip->getPosition() + ofVec2f(0, 20)).distance(lazerIter->getCenter() + ofVec2f(0, 20));

		//float lazerLeft = lazerIter->getPosition().x,
		//	  lazerRight = lazerIter->getPosition().x + lazerIter->getWidth();

		if (distance < 55) {
			hitNoise2.play();
			lazerIter->setHitAgain(false);
			lazerIter->setLifespan(0);
			score -= 50;
			lives--;
			explosionEmitter.setPosition((GummiShip->getPosition() + lazerIter->getCenter()) / 2);
			explosionEmitter2.setPosition(explosionEmitter.getPosition());
			emitDamage();
		}
	}

	// Iterate through the player's lazers
	for (vector<Sprite>::iterator lazerIter = GummiShip->sys->sprites.begin(); lazerIter != GummiShip->sys->sprites.end(); lazerIter++) {
		
		// Checks if a part is already hit
		bool hits = false;

		// Values to check collision on the body
		ofVec2f center = lazerIter->getCenter();
		ofVec2f position = Heartless->getPosition();
		float distance = center.distance(position);
		float height = Heartless->height;
		float contact = height / 2 + lazerIter->getHeight() / 2 - 25;

		// Values to check collision on the arms
		ofVec2f armsCenter = ofVec2f(Heartless->getArmWidth() / 2, Heartless->getArmHeight() / 2);
		float armsContact = lazerIter->getHeight() / 2 + armsCenter.y - 40;
		float leftDistance = center.distance(Heartless->leftArmPos + armsCenter);
		float rightDistance = center.distance(Heartless->rightArmPos + armsCenter);

		// If the boss still has arms, check for 
		// collision on them first before the body
		if (Heartless->hasLeft() || Heartless->hasRight()) {
			
			// Left arm is hit
			if (Heartless->hasLeft() && leftDistance < armsContact) {
				hits = true;
				Heartless->reduceLeftLives();
				if (Heartless->getLeftLives() == 0) {
					Heartless->loseLeft();
					explodeNoise.play();
					emitYellow();
					emitRed();
					score += 4000;
				}
				else {
					hitNoise.play();
					emitBlue();
					emitWhite();
					score += 30;
				}
			}
			// Right arm is hit
			else if (!hits && Heartless->hasRight() && rightDistance < armsContact) {
				hits = true;
				Heartless->reduceRightLives();
				if (Heartless->getRightLives() == 0) {
					Heartless->loseRight();
					explodeNoise.play();
					emitYellow();
					emitRed();
					score += 4000;
				}
				else {
					hitNoise.play();
					emitBlue();
					emitWhite();
					score += 30;
				}
			}

			// An arm is hit so the lazer is affected
			if (hits) {
				lazerIter->setLifespan(0);
				explosionEmitter.setPosition(lazerIter->getCenter() - ofVec2f(0, 30));
				explosionEmitter2.setPosition(explosionEmitter.getPosition());
			}
		}

		// Check collision on body
		if (!hits && distance < contact) {
			Heartless->reduceLife();
			lazerIter->setLifespan(0);
			explosionEmitter.setPosition(lazerIter->getCenter() - ofVec2f(0, 20));
			explosionEmitter2.setPosition(explosionEmitter.getPosition());

			// If boss still has arms when health goes below 
			// 50 the leftover damage will be dealt to arms.
			// Boss cannot die until all parts are dead.
			// This condition is used to force the boss into the
			// Frenzy state when it loses both arms.
			if (Heartless->getLives() < 50 && (Heartless->hasLeft() || Heartless->hasRight())) {
				Heartless->setLives(Heartless->getLives() + 1);
				emitWhite();
				emitBlue();
				hitNoise.play();
				score += 50;

				if (Heartless->getLeftLives() > 0)
					Heartless->reduceLeftLives();
				else
					Heartless->reduceRightLives();

				if (Heartless->getLeftLives() == 0) {
					Heartless->loseLeft();
					Heartless->reduceLeftLives();
					emitRed();
					emitYellow();
				}
				if (Heartless->getRightLives() == 0) {
					Heartless->loseRight();
					Heartless->reduceRightLives();
					emitRed();
					emitYellow();
				}
				
			}
			else if (Heartless->getLives() == 0) {
				explodeNoise.play();
				score += 10000;
				explosionEmitter.setPosition(Heartless->getPosition());
				explosionEmitter2.setPosition(explosionEmitter.getPosition());
				bossExplode();
			}
			else {
				hitNoise.play();
				score += 50;
				emitWhite();
				emitBlue();
			}
		}

	}

	// Check for body collision during tackle
	float distance = (GummiShip->getPosition() + ofVec2f(0, 40)).distance(Heartless->getPosition());
	float contact = Heartless->height / 2 + 114 / 2 - 20;

	if (distance < contact && Heartless->getHitAgain() && Heartless->isAttacking()) {
		crashNoise.play();
		Heartless->setHitAgain(false);
		score -= 50;
		lives--;
		explosionEmitter.setPosition((GummiShip->getPosition() + Heartless->getPosition()) / 2);
		explosionEmitter2.setPosition(explosionEmitter.getPosition());
		emitDamage();
	}

}

// Gets rid of any straggling enemies and explodes at their position
void ofApp::cleanSlateProtocol() {
	
	for (Emitter *m : enemy1s) {
		if (m->sys->sprites.size() != 0) {
			explosionEmitter.setPosition(m->sys->sprites.at(0).getPosition());
			m->sys->sprites.at(0).setLifespan(0);
			m->sys->update();
			emitRed();
			return;
		}
	}
	for (Emitter *m : enemy2s) {
		if (m->sys->sprites.size() != 0) {
			explosionEmitter.setPosition(m->sys->sprites.at(0).getPosition());
			m->sys->sprites.at(0).setLifespan(0);
			m->sys->update();
			emitRed();
			return;
		}
	}
	for (Emitter *m : enemy3sleft) {
		if (m->sys->sprites.size() != 0) {
			explosionEmitter.setPosition(m->sys->sprites.at(0).getPosition());
			m->sys->sprites.at(0).setLifespan(0);
			m->sys->update();
			emitRed();
			return;
		}
	}
	for (Emitter *m : enemy3sright) {
		if (m->sys->sprites.size() != 0) {
			explosionEmitter.setPosition(m->sys->sprites.at(0).getPosition());
			m->sys->sprites.at(0).setLifespan(0);
			m->sys->update();
			emitRed();
			return;
		}
	}
}

void ofApp::emitDamage() {
	explosionEmitter.sys->reset();
	explosionEmitter.setVelocity(ofVec3f(140, 0, 0));
	explosionEmitter.setGroupSize(18);
	explosionEmitter.particleColor = ofColor::red;
	explosionEmitter.start();
	explosionEmitter2.sys->reset();
	explosionEmitter2.setVelocity(ofVec3f(120, 0, 0));
	explosionEmitter2.setGroupSize(10);
	explosionEmitter2.particleColor = explodeColor1;
	explosionEmitter2.start();
}

void ofApp::bossExplode() {
	explosionEmitter.sys->reset();
	explosionEmitter.setVelocity(ofVec3f(240, 0, 0));
	explosionEmitter.setGroupSize(24);
	explosionEmitter.particleColor = ofColor::red;
	explosionEmitter.start();
	explosionEmitter2.sys->reset();
	explosionEmitter2.setVelocity(ofVec3f(200, 0, 0));
	explosionEmitter2.setGroupSize(16);
	explosionEmitter2.particleColor = explodeColor1;
	explosionEmitter2.start();
}

void ofApp::emitRed() {
	explosionEmitter.sys->reset();
	explosionEmitter.setVelocity(ofVec3f(140, 0, 0));
	explosionEmitter.setGroupSize(20);
	explosionEmitter.particleColor = explodeColor1;
	explosionEmitter.start();
}

void ofApp::emitYellow() {
	explosionEmitter2.sys->reset();
	explosionEmitter2.setVelocity(ofVec3f(120, 0, 0));
	explosionEmitter2.setGroupSize(16);
	explosionEmitter2.particleColor = explodeColor2;
	explosionEmitter2.start();
}

void ofApp::emitWhite() {
	explosionEmitter2.sys->reset();
	explosionEmitter2.setVelocity(ofVec3f(70, 0, 0));
	explosionEmitter2.setGroupSize(8);
	explosionEmitter2.particleColor = ofColor(255, 255, 255);
	explosionEmitter2.start();
}

void ofApp::emitBlue() {
	explosionEmitter.sys->reset();
	explosionEmitter.setVelocity(ofVec3f(90, 0, 0));
	explosionEmitter.setGroupSize(13);
	explosionEmitter.particleColor = ofColor(130, 218, 255);
	explosionEmitter.start();
}
