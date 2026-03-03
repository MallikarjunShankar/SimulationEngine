#pragma once

#include <vector>
#include <memory>
#include "Entity.h"
#include "System.h"

class SpawnSystem;
class World {
public:
	World();

	void update(float dt);	
	void render(sf::RenderWindow& window);
	void enqueueSpawn(const Vec2& position);

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

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<System>> systems;
	int nextId;
	float width = 1280.f;
	float height = 720.f;
	SpawnSystem* spawnSystem = nullptr;

private:
	float simulationTime = 0.f;
	float timeScale = 1.f;
	bool paused = false;
	bool pauseToggleRequested = false;

};
