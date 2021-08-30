#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine
	Wrapper class
*/
using namespace sf;
/*
	Class that acts as the game engine
	Wrapper class
*/

class Game
{
private:
	//Variables
	//Window
	RenderWindow* window;
	VideoMode videoMode;
	Event event;

	//Mouse positions
	Vector2i mousePosWindow;
	Vector2f mousePosView;

	//Resources
	Font font;

	//Text
	Text uiText;

	//Game logic
	bool endGsme;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;
	//game objects
	std::vector<RectangleShape> enemies;
	RectangleShape enemy;

	//private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePosition();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(RenderTarget& target);
	void renderEnemies(RenderTarget& target);
	void render();
};

