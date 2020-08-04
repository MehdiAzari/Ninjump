#include "SBDL.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <string>


#define leftLadderPos 45
#define rightLadderPos (420)
#define _CRT_SECURE_NO_WARNINGS
#define ninjaDefaultHeight 380
#define ropeWidth 383
#define thorwerFirstY -288
#define birdWarningTime 200
using namespace std;

const int window_width = 480;
const int window_height = 580;

int back1_speed = 3;
int shieldTime = 1000;
int BladeTime = 500;
int scoreValue = 0;

bool isSoundOn = true;
// increasing AnimationSpeed's Value Slowers the Animation's speed :)))

int ninjaAnimationSpeed = 6;
int sqAnimationSpeed = 6;

struct obstacle {
	bool isAlive = true;
	Texture* tx = nullptr; // texture
	SDL_Rect rect;
};

enum Status {
	playing,
	pause,
	menu,
	lost,
	//Ninja Status:
	running_left,
	running_right,
	jumping,
};

Status NinjaStatus = running_left; // default ninja's position
Status gameStatus = playing;

Texture back1;
Texture foreground;
Texture texture_rope; // 512 * 216
Texture texture_spikes;
Texture thrower;
Texture texture_shield;
Texture back0;
Texture scorebar;

//button
Texture button_tryAgain;
Texture button_continue;
Texture button_menu;
Texture button_setting;
Texture button_play;
Texture button_exit;
Texture button_unmute;
Texture button_mute;
Texture scoreText;
Texture button_sound;
Texture off_slash;

SDL_Rect button_continue_rect = { 180,190,131,45 };
SDL_Rect button_try_rect = { 180,130,131,45 };
SDL_Rect button_menu_rect = { 180,250,131,45 };
SDL_Rect button_play_rect = { 180,130,131,45 };
SDL_Rect button_setting_rect = { 180,190,131,45 };
SDL_Rect button_exit_rect = { 180,250,131,45 };
SDL_Rect button_back_rect = { 180,20,131,45 };
SDL_Rect rect_sound = { 200,450,50,50 };
SDL_Rect rect_off = { 200,450,50,50 };


Texture* CurrentNinja = nullptr; // points to running ninja's array or shieldRuinning's array
Texture* texture_ninja = new Texture[11];
Texture* texture_ninja_shield = new Texture[11];
Texture* texture_sq = new Texture[5];
Texture* texture_bird = new Texture[4];
Texture* texture_blade = new Texture[5];
Texture* texture_balcony = new Texture[3];

Font* scoreFont;

obstacle* obs_sq = new obstacle[3];

SDL_Rect rect_back1_1 = { 0, window_height * -1 ,window_width,window_height * 3 };
SDL_Rect rect_back1_2 = { 0, window_height * -3 ,window_width,window_height * 3 };
SDL_Rect rect_foreground = { 0 , 250 , window_width , window_height };
SDL_Rect rect_ninja_run = { leftLadderPos,ninjaDefaultHeight ,35,70 };
SDL_Rect rect_ninja_jump = { leftLadderPos,ninjaDefaultHeight ,100,60 };
SDL_Rect rect_thrower = { 32, thorwerFirstY ,60,60 };
SDL_Rect rect_scorebar = { 150, 0, 180,60 };
SDL_Rect rect_spike = { leftLadderPos - 2  ,-700 , 25 ,75 };

SDL_Rect* rope = new SDL_Rect[2];
SDL_Rect* rect_balcony = new SDL_Rect[3];
SDL_Rect* rect_sq = new SDL_Rect[2]; // 90 * 50 txt

obstacle* squirrel = new obstacle[2];
obstacle* BladesObs = new obstacle[2];
obstacle shieldObs;
obstacle floatingBlade;
obstacle birdObs;

Music* bgmusic;
Sound* sound_gameover;
Sound* sound_fall_1;
Sound* sound_jump;

bool isShieldActive = false;
//***********************************************************************  Functions  ***************************************************************************************
void initSqRope();
void showBalcony();
void showSpikes();
void obstacleSq();
void showBack1();
void showNinja();
void moveNinja();
void showThrower();
void showFloatingBlade();
void spawnShield();
void showBird();
void showScore();
void ObstacleIntersection(obstacle &obs);
void restartGame();
void initObjects() {

	// loading global textures and Rects or Other things
	// this function is called after SBDL::initEnging()
	
	back1 = SBDL::loadTexture("assets/background/back1.png");
	back0 = SBDL::loadTexture("assets/background/back.jpg");
	foreground = SBDL::loadTexture("assets/background/foreground.png");
	texture_rope = SBDL::loadTexture("assets/obstacles/rope/rope.png");
	texture_spikes = SBDL::loadTexture("assets/obstacles/spike/spikes.png");
	thrower = SBDL::loadTexture("assets/obstacles/thrower/thrower.png");
	texture_shield = SBDL::loadTexture("assets/Shield/shield.png");
	scorebar = SBDL::loadTexture("assets/scorebar.png");

	off_slash = SBDL::loadTexture("assets/menu/off_slash.png");
	button_sound = SBDL::loadTexture("assets/menu/sound_btn.png");
	button_tryAgain = SBDL::loadTexture("assets/but_try.png");
	button_continue = SBDL::loadTexture("assets/but_con.png");
	button_menu = SBDL::loadTexture("assets/butmenu.png");
	button_setting = SBDL::loadTexture("assets/butset.png");
	button_play = SBDL::loadTexture("assets/butplay.png");
	button_exit = SBDL::loadTexture("assets/butexit.png");
	///////////////////
	bgmusic = SBDL::loadMusic("assets/sound/music.ogg");
	sound_fall_1 = SBDL::loadSound("assets/sound/fall_1.wav");
	sound_jump = SBDL::loadSound("assets/sound/jump.wav");
	sound_gameover = SBDL::loadSound("assets/sound/gameover.wav");

	/////////
	scoreFont = SBDL::loadFont("assets/font/azuk.ttf", 26);
	// Loading Textures

	char* textureAddress = new char[100];
	for (int i = 1; i <= 10; i++)
	{
		sprintf(textureAddress, "assets/run/%d.png", i);
		texture_ninja[i] = SBDL::loadTexture(textureAddress);
	}

	for (int i = 1; i <= 10; i++)
	{
		sprintf(textureAddress, "assets/ShieldRunning/b%d.png", i);
		texture_ninja_shield[i] = SBDL::loadTexture(textureAddress);
	}

	for (int i = 1; i <= 4; i++)
	{
		sprintf(textureAddress, "assets/obstacles/squirrels/SQ%d.png", i);
		texture_sq[i] = SBDL::loadTexture(textureAddress);
	}

	for (int i = 1; i <= 3; i++)
	{
		sprintf(textureAddress, "assets/obstacles/bird/bird%d.png", i);
		texture_bird[i] = SBDL::loadTexture(textureAddress);
	}

	for (int i = 1; i <= 4; i++)
	{
		sprintf(textureAddress, "assets/obstacles/blade/%d.png", i);
		texture_blade[i] = SBDL::loadTexture(textureAddress);
	}
	

	for (int i = 1; i <= 2; i++)
	{
		sprintf(textureAddress, "assets/obstacles/houses/b%d.png", i);
		texture_balcony[i] = SBDL::loadTexture(textureAddress);
	}
	

	squirrel[0].tx = texture_sq;
	squirrel[1].tx = texture_sq;

	BladesObs[0].tx = texture_blade;
	BladesObs[1].tx = texture_blade;

	BladesObs[0].rect = { 0 ,thorwerFirstY , 20 , 20 };
	BladesObs[1].rect = { 0 ,thorwerFirstY , 20 , 20 };

	int randPos = -rand() % 2400 - 100;
	rope[0] = { leftLadderPos + 2 ,randPos,ropeWidth,80 };
	squirrel[0].rect = { 0 ,randPos - 23,70,40 };

	randPos = -rand() % 2400 - 100;
	rope[1] = { leftLadderPos + 2 ,randPos,ropeWidth,80 }; 
	squirrel[1].rect = { 0 ,randPos - 23,70,40 };

	shieldObs.rect = { rightLadderPos - 50, -900 ,50,50 };
	
	floatingBlade.rect = { window_width , 0 ,20,20 };
	floatingBlade.tx = texture_blade;

	rect_balcony[1] = { 0 ,-9000 , 80 , 65 };
	rect_balcony[2] = { window_width - 115  ,-6000 , 135 , 45 };

	birdObs.rect = { 400,-1600 , 40 , 40 };
	birdObs.tx = texture_bird;
	
}

//************************************************************************************************************************************************************\\

int main(int argc, char* argv[])
{
	srand(time(NULL));

	

	SBDL::InitEngine("SBDL Test", window_width, window_height);
	initObjects();
	SBDL::playMusic(bgmusic, -1);
	CurrentNinja = texture_ninja;
	
	
	const int FPS = 60; //frame per second
	const int delay = 1000 / FPS; //delay we need at each frame

	
	
	// MAIN LOOP
	while (SBDL::isRunning()) {

		int startTime = SBDL::getTime();
	
		SBDL::updateEvents();
		SBDL::clearRenderScreen();

		switch (gameStatus)
		{

			case menu:
			{
				SBDL::showTexture(back0, 0, 0);
				SBDL::showTexture(button_play, button_play_rect);
				SBDL::showTexture(button_exit, button_exit_rect);

				SBDL::showTexture(button_sound, rect_sound);
				if (!isSoundOn)
				{
					SBDL::showTexture(off_slash, rect_off);
				}

				if (SBDL::mouseInRect(button_play_rect))
				{
					if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
					{
						gameStatus = playing;
						restartGame();
						break;

					}
				}

				if (SBDL::mouseInRect(rect_sound))
				{
					if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
					{
						isSoundOn = !isSoundOn;
						if (isSoundOn)
						{
							SBDL::playMusic(bgmusic, -1);
						}
						else
						{
							SBDL::stopMusic();
						}
					}
				}

				if (SBDL::mouseInRect(button_exit_rect))
				{
					if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
					{
						return 0;
					}
				}
				break;
			}

			case pause:
			{
				SBDL::showTexture(back0, 0, 0);
		
				SBDL::showTexture(button_continue, 180, 190);
				SBDL::showTexture(button_menu, 180, 250);
				if (SBDL::mouseInRect(button_continue_rect))
				{
					if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
					{
						gameStatus = playing;
					}
				}

				if (SBDL::keyPressed(SDL_SCANCODE_P))
				{
					gameStatus = playing;

				}


				if (SBDL::mouseInRect(button_menu_rect))
				{
					if (SBDL::Mouse.clicked(SDL_BUTTON_LEFT, 1, SDL_PRESSED))
					{
					
						gameStatus = menu;
					}
				}
				break;
			}

			case playing:
			{
				if (SBDL::keyPressed(SDL_SCANCODE_P))
				{
					gameStatus = pause;

				}

				showBack1();
				showNinja();
				moveNinja();
				initSqRope();
				obstacleSq();
				showThrower();
				spawnShield();
				showBalcony();
				showSpikes();
				showFloatingBlade();
				showBird();
				showScore();

				break;
			}
		}
		

		SBDL::updateRenderScreen();

		int elapsedTime = SBDL::getTime() - startTime;
		if (elapsedTime < delay)
			SBDL::delay(delay - elapsedTime);
	}
}

void showBack1() {

	SBDL::showTexture(back1, rect_back1_1);
	rect_back1_1.y += back1_speed;

	SBDL::showTexture(back1, rect_back1_2);
	rect_back1_2.y += back1_speed;

	// reseting background position 
	if (rect_back1_1.y >= window_height)
		rect_back1_1.y = (window_height * -3) + back1_speed;

	if (rect_back1_2.y >= window_height)
		rect_back1_2.y = (window_height * -3) + back1_speed;

	SBDL::showTexture(foreground, rect_foreground);
	rect_foreground.y += back1_speed;
}
void showThrower() {
	static int throwPosition = 10;
	SBDL::showTexture(thrower, rect_thrower);

	rect_thrower.y += back1_speed;
	BladesObs[0].rect.y += back1_speed;
	BladesObs[1].rect.y += back1_speed;
	
	int index = rand() % 2 + 3;

	//starts throwing
	if (rect_thrower.y > throwPosition)
	{
		if (BladesObs[0].isAlive)
		{
			SBDL::showTexture(BladesObs[0].tx[index], BladesObs[0].rect);
			BladesObs[0].rect.y += 2;
			BladesObs[0].rect.x += 6;
		}

		if (BladesObs[1].isAlive)
		{
			SBDL::showTexture(BladesObs[1].tx[index], BladesObs[1].rect);
			BladesObs[1].rect.y += 3;
			BladesObs[1].rect.x += 5;
		}
	}

	// check intersection
	if (NinjaStatus == jumping)
	{
		// balde disapreas
		if (SBDL::hasIntersectionRect(rect_ninja_jump, BladesObs[0].rect) && BladesObs[0].isAlive)
		{
			BladesObs[0].isAlive = false;
			scoreValue += 1500;
		}

		if (SBDL::hasIntersectionRect(rect_ninja_jump, BladesObs[1].rect) && BladesObs[1].isAlive)
		{
			BladesObs[1].isAlive = false;
			scoreValue += 1500;
		}
	}
	else {
		ObstacleIntersection(BladesObs[0]);
		ObstacleIntersection(BladesObs[1]);
	}


	//repositioning 
	if (rect_thrower.y > window_height)
	{
		rect_thrower.y = -rand() % 1200 - 300;
		throwPosition = rand() % 50 + 10;

		BladesObs[0].rect.y = rect_thrower.y;
		BladesObs[0].rect.x = rect_thrower.x;
		BladesObs[0].isAlive = true;

		BladesObs[1].rect.y = rect_thrower.y;
		BladesObs[1].rect.x = rect_thrower.x;
		BladesObs[1].isAlive = true;


	}
	// intersection with thrower while ninja is running
	if (SBDL::hasIntersectionRect(rect_ninja_run, rect_thrower)) {

		if (isShieldActive) {
			isShieldActive = false;
			rect_thrower.y += window_height; // cause repositioning 
		}
		else {
			SBDL::playSound(sound_gameover, 1);
			SBDL::playSound(sound_fall_1, 1);
			gameStatus = menu;

		}
	}

}
void initSqRope() {

	// new random position for squirrel and Rope
	for (int i = 0; i < 2; i++)
	{
		if(rope[i].y > window_height)
		{
			int randPos = -rand() % 2400 - 100;
			rope[i] = { leftLadderPos + 2 ,randPos,ropeWidth,80 };
			squirrel[i].rect = { 0 ,randPos - 23,70,40 }; // -23 baes mishe ghashang ro ROPE gharar begire :|
			squirrel[i].isAlive = rand() % 2;
		}
	}

}
void obstacleSq() {
	static int index = 1; // texture's index
	static int counter = 0; // waits then increases the value of index ; makes animation slower

	for (int i = 0; i < 2; i++)
	{
		SBDL::showTexture(texture_rope, rope[i]);

		if(squirrel[i].isAlive)
		{ 
			SBDL::showTexture(squirrel[i].tx[index], squirrel[i].rect);
			if (squirrel[i].rect.y > 100)
			{
				squirrel[i].rect.x+= 4;
			}
		}

		rope[i].y+= back1_speed;
		squirrel[i].rect.y+= back1_speed;

	}
	
	if (counter > sqAnimationSpeed) {
		index++;
		index = index % 4 + 1;
		counter = 0;
	}

	counter++;

	// Intersection part

	if (NinjaStatus == jumping)
	{
			// Squirrels disapear
		if (SBDL::hasIntersectionRect(rect_ninja_jump, squirrel[0].rect) && squirrel[0].isAlive )
		{
			squirrel[0].isAlive = false;
			scoreValue += 1500;
		}

		if (SBDL::hasIntersectionRect(rect_ninja_jump, squirrel[1].rect) && squirrel[1].isAlive)
		{
			squirrel[1].isAlive = false;
			scoreValue += 1500;
		}
	}
	else {
		ObstacleIntersection(squirrel[0]);
		ObstacleIntersection(squirrel[1]);
	}

}
void showNinja() {
	static int index = 0;
	static int counter = 0;

	
	switch (NinjaStatus)
	{
		
		case running_right:
		{
			// 1 to 3
			if (isShieldActive)
			{
				rect_ninja_run.x = rightLadderPos - 58;
			}
			else
				rect_ninja_run.x = rightLadderPos - 24 ;
			SBDL::showTexture(CurrentNinja[index + 1], rect_ninja_run);
			if (counter > ninjaAnimationSpeed)
			{
				index++;
				index %= 3;
				counter = 0;
			}
			counter++;
		
			break;
		}
		
		case running_left:
		{
			// 4 to 6
			if (isShieldActive)
			{
				rect_ninja_run.x = leftLadderPos - 30;
			}
			else
				rect_ninja_run.x = leftLadderPos;
			SBDL::showTexture(CurrentNinja[index + 4], rect_ninja_run);
			if(counter > ninjaAnimationSpeed)
			{
			index++;
			index %= 3;
			counter = 0;
			}
			counter++;
			break;
		}

		case jumping:
		{
			// 7 to 10
			SBDL::showTexture(CurrentNinja[index + 7], rect_ninja_jump);
			if (counter > ninjaAnimationSpeed)
			{
				index++;
				index %= 4;
				counter = 0;
			}
			counter++;
			break;
		}
	}
}
void moveNinja() {
	static int xspeed = -11;
	if (NinjaStatus != jumping) {
		if (SBDL::keyPressed(SDL_SCANCODE_SPACE))
		{
			NinjaStatus = jumping;
			xspeed = -xspeed;
		}
	}
	
	if (NinjaStatus == jumping)
	{
		rect_ninja_jump.x += xspeed;
		rect_ninja_run.x = rect_ninja_jump.x;
	}

	if (rect_ninja_jump.x > rightLadderPos - 30)
		NinjaStatus = running_right;

	if (rect_ninja_jump.x < leftLadderPos)
		NinjaStatus = running_left;	
}

void spawnShield() {
	static int timer = 0;
	

	if (shieldObs.isAlive)
	{
		SBDL::showTexture(texture_shield, shieldObs.rect);
		shieldObs.rect.y += back1_speed;
	}

	if (SBDL::hasIntersectionRect(rect_ninja_run, shieldObs.rect)) {
		shieldObs.rect.y = -rand() % 2000 - 3000;
		isShieldActive = true;
		shieldObs.isAlive = false;
		timer = 0;
	}

	if (isShieldActive && (timer > shieldTime) )
	{
		isShieldActive = false;
		shieldObs.isAlive = true;

	}

	// changes the Texture of Ninja if shield is Active
	//             And Rect's Height & Width
	if (isShieldActive)
	{
		timer++;
		CurrentNinja = texture_ninja_shield;

		rect_ninja_run.h = 100;
		rect_ninja_run.w = 100;
		rect_ninja_jump.h = 80;
		rect_ninja_jump.w = 80;
	}
	else
	{
		CurrentNinja = texture_ninja;

		rect_ninja_run.h = 70;
		rect_ninja_run.w = 35;
		rect_ninja_jump.h = 60;
		rect_ninja_jump.w = 100;

	}
}

void showBalcony() {

	for (int i = 1; i <= 2; i++)
	{
		SBDL::showTexture(texture_balcony[i], rect_balcony[i]);
		rect_balcony[i].y += back1_speed;

		if (rect_balcony[i].y > window_height)
		{
			rect_balcony[i].y = -rand() % 5000 - i * 1000;
		}

		// intersection while ninja is running
		if (SBDL::hasIntersectionRect(rect_ninja_run, rect_balcony[i])) {

			if (isShieldActive) {
				isShieldActive = false;
				rect_balcony[i].y += window_height; // cause repositioning 
			}
			else {
				SBDL::playSound(sound_gameover, 1);
				SBDL::playSound(sound_fall_1, 1);
				gameStatus = menu;

			}
		}
	}


}

void showSpikes() {

	SBDL::showTexture(texture_spikes, rect_spike);
	rect_spike.y += back1_speed;

	if (rect_spike.y > window_height)
	{
		rect_spike.y = -rand() % 5000 - 1000;
	}

	// intersection while ninja is running
	if (SBDL::hasIntersectionRect(rect_ninja_run, rect_spike)) {

		if (isShieldActive) {
			isShieldActive = false;
			rect_spike.y += window_height; // cause repositioning 
		}
		else {
			SBDL::playSound(sound_gameover, 1);
			SBDL::playSound(sound_fall_1, 1);
			gameStatus = menu;

		}
	}
}
void showFloatingBlade() {
	static int BladeTimer = 0;
	static bool isBladeMoving = false;

	if (NinjaStatus == jumping)
	{
		if (SBDL::hasIntersectionRect(rect_ninja_jump, floatingBlade.rect) && floatingBlade.isAlive)
		{
			floatingBlade.isAlive = false;
			scoreValue += 1500;

		}
	}
	else {
		ObstacleIntersection(floatingBlade);
	}
	
	if (floatingBlade.isAlive)
	{
		int index = rand() % 2 + 1;
		SBDL::showTexture(floatingBlade.tx[index], floatingBlade.rect);
	}

	if (BladeTimer > BladeTime)
	{
		isBladeMoving = true;
	}
	BladeTimer++;

	if (isBladeMoving)
	{
		floatingBlade.rect.x -= 4;
		floatingBlade.rect.y += 4;
		
	}

	//repositioning

	if (floatingBlade.rect.x < 0 || floatingBlade.rect.y > window_height)
	{
		floatingBlade.isAlive = true;
		floatingBlade.rect.x = window_width + rand()%30;
		floatingBlade.rect.y = 0;
		isBladeMoving = false;
		BladeTimer = 0;
	}

	
}
void showBird() {
	
	static int index = 1;
	static int counter = 0;
	static int warningTimer = 0;
	if (birdObs.isAlive)
	{
		SBDL::showTexture(birdObs.tx[index], birdObs.rect);
	}

	if (counter > sqAnimationSpeed)
	{
		index++;
		index = index % 3 + 1;
		counter = 0;
	}
	counter++;

	if (birdObs.rect.y < 30) {
		birdObs.rect.y += back1_speed;
	}

	else if (warningTimer < birdWarningTime) {
		warningTimer++;

	}
	else {
		birdObs.rect.x -= 4;
		birdObs.rect.y += back1_speed + 1;

	}

	// repositioning

	if (birdObs.rect.x > window_width || birdObs.rect.y > window_height)
	{
		birdObs.rect.x = 400;
		birdObs.rect.y = -rand() % 3000 - 1000;
		birdObs.isAlive = true;
	}

	// killing obstacle
	if (NinjaStatus == jumping){
		if (SBDL::hasIntersectionRect(rect_ninja_jump, birdObs.rect) && birdObs.isAlive) {

			birdObs.isAlive = false;
			scoreValue += 1500;
		}
	}
	else {
		ObstacleIntersection(birdObs);
	}

}

void ObstacleIntersection(obstacle& obs) {

	// checks intersection with obstacle while running

	if (SBDL::hasIntersectionRect(rect_ninja_run, obs.rect) && obs.isAlive ) {

		if (isShieldActive) {
			isShieldActive = false;
			obs.isAlive = false;
			obs.rect.x = 0;
			obs.rect.y = 0;
		}
		else {
			SBDL::playSound(sound_gameover, 1);
			SBDL::playSound(sound_fall_1, 1);
			gameStatus = menu;

		}
	}

}
void showScore() {

	SBDL::showTexture(scorebar, rect_scorebar);
	scoreValue += back1_speed;
	scoreText = SBDL::createFontTexture(scoreFont, to_string(scoreValue/15), 0, 0, 0);

	SBDL::showTexture(scoreText, 225, 8);

}

void restartGame() {

	scoreValue = 0;

	isShieldActive = true;


}