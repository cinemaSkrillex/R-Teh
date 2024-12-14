#pragma once

#define M_PI 3.14159265358979323846
#define GAME_SCALE 3.0f

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

// Core
#include "Core/View.hpp"
#include "Core/Window.hpp"
// Components
#include "ECS/Components/AI.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Components/AutoDestructible.hpp"
#include "ECS/Components/Collision.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Components/Damage.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Health.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Radius.hpp"
#include "ECS/Components/Rotation.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"
#include "ECS/Components/Target.hpp"
#include "ECS/Components/Velocity.hpp"
// Entities
#include "ECS/Entities/Entity.hpp"
#include "ECS/Entities/ManageEntities.hpp"
// Registry
#include "ECS/Registry/Registry.hpp"
// SparseArrays
#include "ECS/SparseArray/SparseArray.hpp"
// Systems
#include "ECS/Systems/AISystem.hpp"
#include "ECS/Systems/CollisionSystem.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Systems/DestructibleSystem.hpp"
#include "ECS/Systems/DrawSystem.hpp"
#include "ECS/Systems/HealthSystem.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "ECS/Systems/NetvarSystem.hpp"
#include "ECS/Systems/RadiusSystem.hpp"
#include "ECS/Systems/RotationSystem.hpp"
// Media
#include "Media/Audio/Music.hpp"
#include "Media/Audio/Sound.hpp"
#include "Media/Graphics/Rendering/Background.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"
#include "Media/Graphics/UI/Text.hpp"
// Scripting
// #include "Scripting/LuaManager.hpp"