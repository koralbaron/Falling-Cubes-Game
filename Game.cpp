#include "Game.h"

//private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGsme = false;
	this->points = 0;
	this->health = 20;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new RenderWindow(this->videoMode, "My first game", Style::Titlebar | Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts/Dosis-Light.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS::Faild to load font" << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(32);
	this->uiText.setFillColor(Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f,10.f);
	this->enemy.setSize(Vector2f(100.f, 100.f));
	this->enemy.setFillColor(Color::Cyan);
	//this->enemy.setOutlineColor(Color::Green);
	//this->enemy.setOutlineThickness(1.f);
	
}

//Constructors / Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGsme;
}



//Functions
void Game::spawnEnemy()
{
	/*
		@return void

		Spawns enemies and sets their types and colors. Spawns them at random positions.
		-Sets a random type (diff).
		-Set a random position.
		-Set a random color.
		-Add enemy to the vector.
	*/

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);
	//Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(Vector2f(10.f, 10.f));
		this->enemy.setFillColor(Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(Vector2f(30.f, 30.f));
		this->enemy.setFillColor(Color::Blue);
		break;
	case 2:
		this->enemy.setSize(Vector2f(50.f, 50.f));
		this->enemy.setFillColor(Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(Vector2f(70.f, 70.f));
		this->enemy.setFillColor(Color::Red);
		break;
	case 4:
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Green);
		break;
	default:
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Yellow);
		break;
	}
	//Spawn the enemy
	this->enemies.push_back(this->enemy);



}

void Game::pollEvents()
{
	//event polling
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type)
		{
		case Event::Closed:
			this->window->close();
			break;
		case Event::KeyPressed:
			if (this->event.key.code == Keyboard::Escape)
				this->window->close();
			break;
		default:
			break;
		}
	}
}

void Game::updateMousePosition()
{
	/*
	@ return void
	Updates the mouse positions:
		- Mouse position relative to window (Vector2i)
	*/

	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";
	this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
	/*
		@return void

		Update the enemy spawn thimer and spawns enemies
		when the total amount of enemies is smaller that the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen. // TODO
	*/

	//Updating the timer for enemy spwaning
	if (this->enemies.size() < this->maxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//spawn the enemy and reset timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//Moving and updating enemies 
	for (int i = 0; i < this->enemies.size(); i++) {

		bool deleted = false;

		this->enemies[i].move(0.f, 3.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << "\n";
		}
	}

	//Check if clicked upon
	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (this->mouseHeld == false) {
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {

					//Gain points
					if (this->enemies[i].getFillColor() == Color::Magenta)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == Color::Blue)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == Color::Cyan)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == Color::Red)
						this->points += 3;
					else if (this->enemies[i].getFillColor() == Color::Green)
						this->points += 1;

					std::cout << "Points: " << this->points << "\n";

					//Delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else {
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (!this->endGsme) {

		this->updateMousePosition();

		this->updateText();

		this->updateEnemies();
	}
	//End game condition
	if (this->health <= 0)
		this->endGsme = true;
}

void Game::renderText(RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(RenderTarget& target)
{
	//Rendering all the enemies
	for (auto& e : this->enemies) {
		target.draw(e);
	}
}

void Game::render()
{
	/*
	@return void

		- clear old frame
		- render objects
		- display frame in window

		Randers the game objects.
	*/

	this->window->clear();

	//Draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}
