#include "World.h"
#include "Random.h"
#include "ForceSystem.h"
#include "SpawnSystem.h"
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

World::World() {
	auto spawn = std::make_unique<SpawnSystem>(*this);

	spawnSystem = spawn.get();
	systems.push_back(std::move(spawn));

	systems.push_back(std::make_unique<ForceSystem>());
}

void World::enqueueSpawn(const Vec2& position) {

	Vec2 velocity(
		Random::range(-100.f, 100.f),
		Random::range(-100.f, 100.f)
	);

	recordSpawn(position, velocity);

	spawnSystem->enqueue({ position, velocity });
}

void World::update(float dt) {

	if (replayMode) {
		injectReplayInputs();
	}

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

	if (resetTimeScaleRequested) {
		timeScale = 1.f;
		resetTimeScaleRequested = false;
	}

	if (pendingTimeScaleDelta != 0.f) {
		recordInput(InputCommand::TimeScaleDelta);
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

	recordStateHash();
	frameIndex++;
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

uint64_t World::getFrameIndex() const {
	return frameIndex;
}

void World::requestPauseToggle() {
	recordInput(InputCommand::Pause);
	pauseToggleRequested = true;
}

void World::requestSingleStep() {
	recordInput(InputCommand::Step);
	stepRequested = true;
}

void World::requestTimeScaleDelta(float delta) {

	if (!replayMode) {
		if (delta > 0.f) {
			recordInput(InputCommand::TimeScaleUp);
		} else {
			recordInput(InputCommand::TimeScaleDown);
		}
	}

	pendingTimeScaleDelta += delta;
}

void World::requestTimeScaleReset() {
	recordInput(InputCommand::TimeScaleReset);
	resetTimeScaleRequested = true;
}

void World::recordInput(InputCommand command) {
	if (replayMode) {
		return;
	}

	inputLog.push_back({ frameIndex + 1, command, Vec2()});
}

void World::startReplay() {
	resetWorld();

	replayCursor = 0;
	replayMode = true;
}

void World::stopReplay() {
	replayMode = false;
}

void World::injectReplayInputs() {
	while (replayCursor < inputLog.size() && inputLog[replayCursor].frame == frameIndex) {
		const auto& entry = inputLog[replayCursor];

		switch (entry.command)
		{
		case InputCommand::Spawn:
			spawnSystem->enqueue({ entry.position, entry.velocity });
			break;

		case InputCommand::Pause:
			pauseToggleRequested = true;
			break;

		case InputCommand::Step:
			stepRequested = true;
			break;

		case InputCommand::TimeScaleUp:
			pendingTimeScaleDelta += 0.25f;
			break;

		case InputCommand::TimeScaleDown:
			pendingTimeScaleDelta -= 0.25f;
			break;

		case InputCommand::TimeScaleReset:
			resetTimeScaleRequested = true;
			break;
		}

		replayCursor++;
	}

	if (replayCursor >= inputLog.size()) {
		replayMode = false;
	}
}

void World::recordSpawn(const Vec2& position, const Vec2& velocity) {

	if (replayMode)
		return;

	inputLog.push_back({
		frameIndex + 1,
		InputCommand::Spawn,
		position,
		velocity
		});
}

bool World::isReplayMode() const {
	return replayMode;
}

void World::resetWorld()
{
	entities.clear();

	frameIndex = 0;
	simulationTime = 0.f;

	paused = false;
	pauseToggleRequested = false;
	stepRequested = false;

	timeScale = 1.f;

	replayCursor = 0;

	replayHashes.clear();
}

uint64_t World::computeStateHash() const {
	uint64_t hash = 1469598103934665603ULL;

	for (const auto& e : entities) {
		const Entity& entity = *e;

		auto mix = [&](uint64_t value) {
			hash ^= value;
			hash *= 1099511628211ULL;
			};

		mix(entity.id);
		
		mix(static_cast<uint64_t>(entity.position.x * 1000));
		mix(static_cast<uint64_t>(entity.position.y * 1000));

		mix(static_cast<uint64_t>(entity.velocity.x * 1000));
		mix(static_cast<uint64_t>(entity.velocity.y * 1000));

		mix(static_cast<uint64_t>(entity.energy));

		mix(entity.alive ? 1 : 0);
	}
	return hash;
}

void World::recordStateHash() {
	uint64_t hash = computeStateHash();

	if (!replayMode) {
		stateHashes.push_back(hash);
	}
	else {
		replayHashes.push_back(hash);
		validateReplayHash(hash);
	}
}

void World::validateReplayHash(uint64_t hash) {

	if (frameIndex == 0) {
		return;
	}

	size_t index = frameIndex - 1;

	if (index >= stateHashes.size()) {
		return;
	}

	uint64_t expected = stateHashes[index];

	if (hash != expected) {
		std::cout << "Determinism failure at frame " << frameIndex << std::endl;
		std::cout << "Expected: " << expected << std::endl;
		std::cout << "Actual: " << hash << std::endl;
	}
}

size_t World::getEntityCount() const
{
	return entities.size();
}