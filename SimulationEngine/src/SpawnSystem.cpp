#include "SpawnSystem.h"
#include "World.h"
#include "Entity.h"

SpawnSystem::SpawnSystem(World& world)
	: world(world)
{
}

void SpawnSystem::enqueue(const SpawnRequest& request) {
	queue.push_back(request);
}

void SpawnSystem::update(
	std::vector<std::unique_ptr<Entity>>& entities,
	float /*dt*/) {
	for (const auto& req : queue) {
		auto entity = std::make_unique<Entity>();

		entity->position = req.position;
		entity->velocity = req.velocity;

		entities.push_back(std::move(entity));
	}

	queue.clear();
}