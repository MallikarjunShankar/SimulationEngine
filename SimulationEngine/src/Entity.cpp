#include "Entity.h"
#include <SFML/Graphics.hpp>

Entity::Entity()
    :
    id(0),
    position(0.f, 0.f),
    velocity(0.f, 0.f),
    acceleration(0.f, 0.f),
    mass(1.f),
    radius(5.f),
    energy(100.f),
    alive(true)
{
}

void Entity::applyForce(const Vec2& force)
{
    acceleration += force / mass;
}

void Entity::update(float dt)
{
    velocity += acceleration * dt;
    float factor = 1.f - linearDamping * dt;
    if (factor < 0.f) factor = 0.f;

	velocity *= factor;

    position += velocity * dt;

    acceleration = Vec2(0.f, 0.f);
}

void Entity::render(sf::RenderWindow& window)
{
    sf::CircleShape shape(radius);

    shape.setOrigin({ radius, radius });
    shape.setPosition({ position.x, position.y });
    shape.setFillColor(sf::Color::White);

    window.draw(shape);
}