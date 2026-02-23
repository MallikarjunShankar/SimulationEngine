#include "App.h"

App::App()
:	window(sf::VideoMode({1280, 720}), "Simulation Engine")
{
	window.setFramerateLimit(144);

	world.spawnEntity();
}

void App::run() {

	const float dt = 1.f / 120.f;

	sf::Clock clock;
	float accumulator = 0.f; 

	while (window.isOpen()) {

		float frameTime = clock.restart().asSeconds();
		accumulator += frameTime;

		processEvents();

		while (accumulator >= dt) {
			update(dt);
			accumulator -= dt;
		}
		
		render();
	}
}

void App::processEvents() {
	while (auto event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

void App::update(float dt) {
	world.update(dt);
}

void App::render() {
	window.clear();
	world.render(window);
	window.display();
}