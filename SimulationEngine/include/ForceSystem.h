#pragma once
#include "System.h"

class ForceSystem : public System
{
public:
    void update(
        std::vector<std::unique_ptr<Entity>>& entities,
        float dt) override;
};