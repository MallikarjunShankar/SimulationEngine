#pragma once

#include <SFML/Graphics.hpp>
#include "World.h"
#include "OperatorController.h"

class App {
public:
	App();
	void run();

private:
	void processEvents();
	void update(float dt);
	void render();

private:
	sf::RenderWindow window;
	World world;
	sf::Font debugFont;
	sf::Text debugText{ debugFont };
	OperatorController controller;
};