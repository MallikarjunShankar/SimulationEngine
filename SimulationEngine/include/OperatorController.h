#pragma once

#include <SFML/Window/Event.hpp>

class World;

class OperatorController {
public:
    explicit OperatorController(World& world);
    void handleKey(const sf::Event::KeyPressed& key);

private:
    World& world;
};