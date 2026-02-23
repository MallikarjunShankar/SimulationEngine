#include "World.h"
#include "Random.h"
#include <algorithm>
#include <SFML/Graphics.hpp>

World::World()
	:nextId(0)
{
}

void World::spawnEntity() {
	auto entity = std::make_unique<Entity>();
	
	entity->id = nextId++;
	entity->position = Vec2(640.f, 360.f);

	entities.push_back(std::move(entity));
}

void World::update(float dt) {
	for (auto& entity : entities) {
		Vec2 force(50.f, 0.f);

		entity->applyForce(force);
		entity->update(dt);
	}

	cleanup();
}

void World::cleanup() {
	entities.erase(
		std::remove_if(
			entities.begin(),
			entities.end(),
			[](const std::unique_ptr<Entity>& e) {
				return !e->alive;
			}
		),
		entities.end()
	);
}

void World::render(sf::RenderWindow& window) {
	for (auto& entity : entities) {
		entity->render(window);
	}
}

void World::spawnAt(const Vec2& position) {
	auto entity = std::make_unique<Entity>();

	entity->position = position;

	entity->velocity = Vec2(
		Random::range(-100.f, 100.f),
		Random::range(-100.f, 100.f)
	);

	entities.push_back(std::move(entity));
}