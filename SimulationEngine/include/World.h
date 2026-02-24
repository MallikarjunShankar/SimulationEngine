#pragma once

#include <vector>
#include <memory>
#include "Entity.h"
#include "ForceSystem.h"
#include "System.h"

class World {
public:
	World();

	void update(float dt);	
	void spawnEntity();
	void render(sf::RenderWindow& window);
	void spawnAt(const Vec2& position);

private:
	void cleanup();
	void resolveBounds(Entity& entity);

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<System>> systems;
	int nextId;
	float width = 1280.f;
	float height = 720.f;

};