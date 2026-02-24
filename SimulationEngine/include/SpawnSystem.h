#pragma once

#include "System.h"
#include "SpawnRequest.h"
#include <vector>

class World;
class SpawnSystem : public System {
public:
	SpawnSystem(World& world);

	void enqueue(const SpawnRequest& request);

	void update(
		std::vector<std::unique_ptr<Entity>>& entities,
		float dt) override;
	
private:
	World& world;
	std::vector<SpawnRequest> queue;
};