#include "OperatorController.h"
#include "World.h"

OperatorController::OperatorController(World& world)
    : world(world) {
}

void OperatorController::handleKey(const sf::Event::KeyPressed& key) {
    switch (key.code) {
    case sf::Keyboard::Key::Space:
        world.requestPauseToggle();
        break;

    case sf::Keyboard::Key::Right:
        world.requestSingleStep();
        break;

    case sf::Keyboard::Key::Up:
        world.requestTimeScaleDelta(0.25f);
        break;

    case sf::Keyboard::Key::Down:
        world.requestTimeScaleDelta(-0.25f);
        break;

    case sf::Keyboard::Key::R:
        world.requestTimeScaleReset();
        break;

    case sf::Keyboard::Key::C:
        world.resetWorld();
        break;

    case sf::Keyboard::Key::T:
        if (world.isReplayMode()) {
            world.stopReplay();
        } else {
            world.startReplay();
        }
        break;

    default:
        break;
    }
}