#include "ForceSystem.h"
#include "Entity.h"
#include "Vec2.h"

void ForceSystem::update(
    std::vector<std::unique_ptr<Entity>>& entities,
    float /*dt*/)
{
    Vec2 constantForce(0.f, 50.f);

    for (auto& e : entities)
    {
        e->applyForce(constantForce);
    }
}