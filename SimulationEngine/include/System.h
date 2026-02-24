#pragma once

#include <vector>
#include <memory>

class Entity;

class System
{
public:
    virtual ~System() = default;

    virtual void update(
        std::vector<std::unique_ptr<Entity>>& entities,
        float dt) = 0;
};