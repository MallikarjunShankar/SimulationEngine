#pragma once

#include <vector>
#include <memory>

class Entity;

class ForceSystem {
public:
	void apply(std::vector<std::unique_ptr<Entity>>& entities);
};
