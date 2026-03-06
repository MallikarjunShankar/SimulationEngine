#include <iostream>
#include <sstream>
#include <iomanip>
#include "App.h"
#include "World.h"
#include "OperatorController.h"

App::App()
    : window(sf::VideoMode({ 1280, 720 }), "Simulation Engine"),
    controller(world) {
    window.setFramerateLimit(144);

    if (!debugFont.openFromFile("arial.ttf")) {
        std::cout << "Failed to load font\n";
    }

    debugText.setFont(debugFont);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition({ 10.f, 10.f });
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
    while (auto optEvent = window.pollEvent())
    {
        const sf::Event& event = *optEvent;

        if (const auto* closed = event.getIf<sf::Event::Closed>())
        {
            window.close();
        }

        if (!world.isReplayMode()) {
            if (const auto* mouse =
                event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);

                    Vec2 worldPos(
                        static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y)
                    );

                    world.enqueueSpawn(worldPos);
                }
            }
        }

        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            controller.handleKey(*key);
        }
    }
}

void App::update(float dt) {
    world.update(dt);
}

void App::render() {
    window.clear();
    world.render(window);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << world.getTimeScale();

    std::string overlay =
        "Frame: " + std::to_string(world.getFrameIndex()) +
        "\nEntities: " + std::to_string(world.getEntityCount()) +
        "\nTimeScale: " + ss.str() +
        "\nPaused: " + std::string(world.isPaused() ? "YES" : "NO") +
        "\nReplay: " + std::string(world.isReplayMode() ? "ON" : "OFF");

    debugText.setString(overlay);

    window.draw(debugText);
    window.display();
}