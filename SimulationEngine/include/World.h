#pragma once

#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include "Entity.h"
#include "System.h"

enum class InputCommand {
	Spawn,
	Pause, 
	Step
};

struct RecordedInput {
	uint64_t frame;
	InputCommand command;
	Vec2 position;
};

class SpawnSystem;
class World {
public:
	World();

	void update(float dt);	
	void render(sf::RenderWindow& window);
	void enqueueSpawn(const Vec2& position);
	void recordSpawn(const Vec2& position);

private:
	void cleanup();
	void resolveBounds(Entity& entity);

public:
	void setPaused(bool value);
	bool isPaused() const;
	void requestPauseToggle();
	
	void setTimeScale(float scale);
	float getTimeScale() const;
	float getSimulationTime() const;

	void requestSingleStep();
	void requestTimeScaleDelta(float delta);
	void requestTimeScaleReset();

	uint64_t getFrameIndex() const;
	void recordInput(InputCommand command);

	void startReplay();
	void stopReplay();
	void injectReplayInputs();
	bool isReplayMode() const;

	void resetWorld();
	uint64_t computeStateHash() const;
	void recordStateHash();
	void validateReplayHash(uint64_t hash);
	size_t getEntityCount() const;

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<System>> systems;
	std::vector<RecordedInput> inputLog;
	std::vector<uint64_t> stateHashes;
	std::vector<uint64_t> replayHashes;

	int nextId;
	float width = 1280.f;
	float height = 720.f;
	SpawnSystem* spawnSystem = nullptr;

private:
	float simulationTime = 0.f;
	float timeScale = 1.f;
	bool paused = false;
	bool pauseToggleRequested = false;

	bool stepRequested = false;
	float pendingTimeScaleDelta = 0.f;
	bool resetTimeScaleRequested = false;

	bool replayMode = false;
	size_t replayCursor = 0;

	uint64_t frameIndex = 0;
};