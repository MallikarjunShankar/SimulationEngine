#include "World.h"
#include "Random.h"
#include "ForceSystem.h"
#include "SpawnSystem.h"
#include <algorithm>
#include <SFML/Graphics.hpp>

World::World() {
	auto spawn = std::make_unique<SpawnSystem>(*this);

	spawnSystem = spawn.get();
	systems.push_back(std::move(spawn));

	systems.push_back(std::make_unique<ForceSystem>());
}

void World::enqueueSpawn(const Vec2& position) {
	spawnSystem->enqueue({ position });
}

void World::update(float dt) {
	// systems modify entities
	for (auto& system : systems) {
		system->update(entities, dt);
	}

	// entities integrate motion
	for (auto& e : entities) {
		e->update(dt);
		resolveBounds(*e);
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

void World::resolveBounds(Entity& entity) {
	float r = entity.radius;
	
	// left bound
	if (entity.position.x - r < 0.f) {
		entity.position.x = r;
		entity.velocity.x *= -1.f;
	}

	// right bound
	if (entity.position.x + r > width){
		entity.position.x = width - r;
		entity.velocity.x *= -1.f;
	}

	// top bound
	if (entity.position.y - r < 0.f) {
		entity.position.y = r;
		entity.velocity.y *= -1.f;
	}

	// bottom bound
	if (entity.position.y + r > height) {
		entity.position.y = height - r;
		entity.velocity.y *= -1.f;
	}
}