#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class Entity {
public:
	Entity();

	void applyForce(const Vec2& force);
	void update(float dt);
	void render(sf::RenderWindow& window);

public:
	int id; // unique identifier for the entity

	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	float mass;
	float radius;
	float energy;

	bool alive;
};