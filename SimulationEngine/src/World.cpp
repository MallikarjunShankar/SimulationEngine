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
	recordSpawn(position);
	spawnSystem->enqueue({ position });
}

void World::update(float dt) {
	if (pauseToggleRequested) {
		paused = !paused;
		pauseToggleRequested = false;
	}

	bool shouldAdvance = true;


	if (paused) {
		if (stepRequested) {
			shouldAdvance = true;
			stepRequested = false;
		}

		else {
			shouldAdvance = false;
		}
	}

	if (!shouldAdvance) {
		return;
	}

	frameIndex++;

	if (replayMode) {
		injectReplayInputs();
	}

	if (resetTimeScaleRequested) {
		timeScale = 1.f;
		resetTimeScaleRequested = false;
	}

	if (pendingTimeScaleDelta != 0.f) {
		timeScale += pendingTimeScaleDelta;
		
		if (timeScale < 0.f) timeScale = 0.f;
		if (timeScale > 4.f) timeScale = 4.f;

		pendingTimeScaleDelta = 0.f;
	}

	const float scaledDt = dt * timeScale;
	simulationTime += scaledDt;

	for (auto& system : systems) {
		system->update(entities, scaledDt);
	}

	for (auto& e : entities) {
		e->update(scaledDt);
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

void World::setPaused(bool value) {
	paused = value;
}

bool World::isPaused() const {
	return paused;
}

void World::setTimeScale(float scale) {
	timeScale = scale;
}

float World::getTimeScale() const {
	return timeScale;
}

float World::getSimulationTime() const {
	return simulationTime;
}

void World::requestPauseToggle() {
	recordInput("pause");
	pauseToggleRequested = true;
}

void World::requestSingleStep() {
	recordInput("step");
	stepRequested = true;
}

void World::requestTimeScaleDelta(float delta) {
	pendingTimeScaleDelta += delta;
}

void World::requestTimeScaleReset() {
	resetTimeScaleRequested = true;
}

void World::recordInput(const std::string& command) {
	if (replayMode) {
		return;
	}

	inputLog.push_back({ frameIndex, command });
}

void World::startReplay() {
	resetWorld();
	replayMode = true;
}

void World::stopReplay() {
	replayMode = false;
}

void World::injectReplayInputs() {
	while (replayCursor < inputLog.size() && inputLog[replayCursor].frame == frameIndex) {
		const auto& entry = inputLog[replayCursor];

		if (entry.command == "spawn") {
			spawnSystem->enqueue({ entry.position });
		}

		else if (entry.command == "pause") {
			pauseToggleRequested = true;
		}

		else if (entry.command == "step") {
			stepRequested = true;
		}

		replayCursor++;
	}
}

void World::recordSpawn(const Vec2& position) {
	if (replayMode) return;

	inputLog.push_back({ frameIndex, "spawn", position });
}

bool World::isReplayMode() const {
	return replayMode;
}

void World::resetWorld() {
	entities.clear();

	frameIndex = 0;
	simulationTime = 0.f;

	pauseToggleRequested = false;
	stepRequested = false;

	paused = false;

	replayCursor = 0;
	timeScale = 1.f;
}