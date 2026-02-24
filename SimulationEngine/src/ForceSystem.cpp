#include "ForceSystem.h"
#include "Entity.h"
#include "Vec2.h"

void ForceSystem::apply(std::vector<std::unique_ptr<Entity>>& entities) {
	Vec2 constantForce(0.f, 50.f);

	for (auto& e : entities) {
		e->applyForce(constantForce);
	}
}