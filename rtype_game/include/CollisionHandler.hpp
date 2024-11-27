#pragma once

#include <unordered_map>
#include <iostream>
#include "ECS/Registry/Registry.hpp"
#include "ECS/Components/Collision.hpp"
#include "ECS/Components/Position.hpp"


void handle_collision(Registry& registry, const std::unordered_map<std::string, Entity>& entities);