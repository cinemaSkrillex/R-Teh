/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInit
*/

#include "../include/Game/GameInstance.hpp"

GameInstance::GameInstance()
    : _clock(),
      _window(WINDOWED ? std::make_unique<RealEngine::Window>("I CAN SEE EVERYTHING",
                                                              sf::Vector2u(800, 600))
                       : nullptr),
      _registry(),
      _drawSystem(WINDOWED ? &_window->getRenderWindow() : nullptr),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _netvarSystem() {
    init_components();
    init_systems();
    init_textures();
    init_sprites();
    init_mobs();
}

void GameInstance::init_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::Health>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::SpriteSheet>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::AI>();
    _registry.register_component<RealEngine::Rotation>();
    _registry.register_component<RealEngine::Radius>();
    _registry.register_component<RealEngine::Target>();
    _registry.register_component<RealEngine::AutoDestructible>();
    _registry.register_component<RealEngine::Damage>();
    _registry.register_component<RealEngine::Netvar>();
    _registry.register_component<RealEngine::NetvarContainer>();
}

void GameInstance::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _aiSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _rotationSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _radiusSystem.update(registry);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _healthSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.updateWithoutDisplay(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
}

void GameInstance::init_mobs() {
    // rtype::EyeMinion   eyeMinion(_registry, {100, 100}, {0, 0}, 0);
    // rtype::SpaceSphere spaceSphere(_registry, {100, 100}, {0, 0}, 0);
    addAndGetSimpleMob({1000, 100}, {-1, 0}, 50 * 2, 50);
    addAndGetSimpleMob({2050, 100}, {-1, 0}, 50 * 2, 50);
    addAndGetSimpleMob({2100, 200}, {-1, 0}, 35 * 2, 100);
    addAndGetSimpleMob({1500, 400}, {-1, 0}, 35 * 2, 100);
    addAndGetSimpleMob({1200, 100}, {-1, 0}, 40 * 2, 100);
    addAndGetSimpleMob({1110, 50}, {-1, 0}, 40 * 2, 100);
    addAndGetSimpleMob({1800, 500}, {-1, 0}, 40 * 2, 100);
    addAndGetSimpleMob({1500, 250}, {-1, 0}, 40 * 2, 100);
    addAndGetSimpleMob({1000, 470}, {-1, 0}, 35 * 2, 100);
    addAndGetSimpleMob({1710, 170}, {-1, 0}, 35 * 2, 100);
    addAndGetSimpleMob({1900, 490}, {-1, 0}, 20 * 2, 100);
    addAndGetSimpleMob({1500, 230}, {-1, 0}, 20 * 2, 100);
    addAndGetSimpleMob({1600, 70}, {-1, 0}, 42 * 2, 100);
    addAndGetSimpleMob({1100, 200}, {-1, 0}, 42 * 2, 100);
    addAndGetSimpleMob({1200, 200}, {-1, 0}, 35 * 2, 100);
}

void GameInstance::init_textures() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadTexture("spaceship_up", "../../assets/sprites/spaceship.png",
                                     sf::IntRect{0, 0, 32 * 2, 15});
    AssetManagerInstance.loadTexture("spaceship_idle", "../../assets/sprites/spaceship.png",
                                     sf::IntRect{0, 15, 32, 15});
    AssetManagerInstance.loadTexture("spaceship_down", "../../assets/sprites/spaceship.png",
                                     sf::IntRect{0, 15 * 2, 33 * 2, 15});
    AssetManagerInstance.loadTexture("stars_background",
                                     "../../assets/sprites/backgrounds/stars.png");
    AssetManagerInstance.getTexture("stars_background")->setRepeated(true);
    AssetManagerInstance.loadTexture("bullet", "../../assets/sprites/spaceship_bullet.png");
    AssetManagerInstance.loadTexture("space_plane", "../../assets/sprites/enemies/space_plane.png");
    AssetManagerInstance.loadTexture("space_drill", "../../assets/sprites/enemies/space_drill.png");
    AssetManagerInstance.loadTexture("space_sphere",
                                     "../../assets/sprites/enemies/space_sphere.png");
    AssetManagerInstance.loadTexture("turret_canon",
                                     "../../assets/sprites/enemies/turret_canon.png");
    AssetManagerInstance.loadTexture("turret_pedestal",
                                     "../../assets/sprites/enemies/turret_pedestal.png");
    AssetManagerInstance.loadTexture("mob_spawner_ship",
                                     "../../assets/sprites/enemies/mob_spawner.png");
    AssetManagerInstance.loadTexture("mortar_rocket",
                                     "../../assets/sprites/enemies/mortar_rocket.png");
    AssetManagerInstance.loadTexture("robot_boss_minion",
                                     "../../assets/sprites/enemies/boss_minion.png");
    AssetManagerInstance.loadTexture("robot_boss_shoot",
                                     "../../assets/sprites/enemies/mini_boss.png",
                                     sf::IntRect{0, 0, 47, 43});
    AssetManagerInstance.loadTexture("robot_boss_fordward",
                                     "../../assets/sprites/enemies/mini_boss.png",
                                     sf::IntRect{0, 43, 56, 54});
    AssetManagerInstance.loadTexture("robot_boss_backward",
                                     "../../assets/sprites/enemies/mini_boss.png",
                                     sf::IntRect{0, 97, 49, 50});
    AssetManagerInstance.loadTexture("directional_canon",
                                     "../../assets/sprites/enemies/directional_canon.png");
    AssetManagerInstance.loadTexture("eye_bomber", "../../assets/sprites/enemies/eye_bomber.png");
    AssetManagerInstance.loadTexture("eye_minion", "../../assets/sprites/enemies/eye_minion.png");
    AssetManagerInstance.loadTexture("eye_bigion_normal",
                                     "../../assets/sprites/enemies/eye_bigion.png",
                                     sf::IntRect{0, 0, 23 * 2, 16});
    AssetManagerInstance.loadTexture("eye_bigion_angry",
                                     "../../assets/sprites/enemies/eye_bigion.png",
                                     sf::IntRect{0, 16, 21 * 2, 16});
    AssetManagerInstance.loadTexture("eye_boss_short_range",
                                     "../../assets/sprites/enemies/the_eye/boss.png",
                                     sf::IntRect{0, 0, 73 * 3, 55});
    AssetManagerInstance.loadTexture("eye_boss_mid_range",
                                     "../../assets/sprites/enemies/the_eye/boss.png",
                                     sf::IntRect{0, 55, 91 * 3, 55});
    AssetManagerInstance.loadTexture("eye_boss_long_range",
                                     "../../assets/sprites/enemies/the_eye/boss.png",
                                     sf::IntRect{0, 55 * 2, 81 * 3, 55});
    AssetManagerInstance.loadTexture("eye_laser",
                                     "../../assets/sprites/enemies/the_eye/laser_shoot.png");
    AssetManagerInstance.loadTexture("space_vortex",
                                     "../../assets/sprites/enemies/space_vortex.png");
    AssetManagerInstance.loadTexture("space_laser", "../../assets/sprites/enemies/laser_shoot.png");
    AssetManagerInstance.loadTexture("small_laser", "../../assets/sprites/enemies/laser_shoot.png");
    AssetManagerInstance.loadTexture("fireball", "../../assets/sprites/enemies/fireball.png");
    AssetManagerInstance.loadTexture("big_bullet", "../../assets/sprites/big_shoot.png");
    AssetManagerInstance.loadTexture("mid_bullet", "../../assets/sprites/medium_shoot.png");
}

void GameInstance::init_sprites() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSprite("spaceship_up", "spaceship_up");
    AssetManagerInstance.loadSprite("spaceship_idle", "spaceship_idle");
    AssetManagerInstance.loadSprite("spaceship_down", "spaceship_down");
    AssetManagerInstance.loadSprite("spaceship_other", "spaceship_idle");
    AssetManagerInstance.loadSprite("bullet", "bullet");
    AssetManagerInstance.loadSprite("stars_background", "stars_background");
    AssetManagerInstance.loadSprite("space_plane", "space_plane");
    AssetManagerInstance.loadSprite("space_drill", "space_drill");
    AssetManagerInstance.loadSprite("space_sphere", "space_sphere");
    AssetManagerInstance.loadSprite("turret_canon", "turret_canon");
    AssetManagerInstance.loadSprite("turret_pedestal", "turret_pedestal");
    AssetManagerInstance.loadSprite("mob_spawner_ship", "mob_spawner_ship");
    AssetManagerInstance.loadSprite("mortar_rocket", "mortar_rocket");
    AssetManagerInstance.loadSprite("robot_boss_minion", "robot_boss_minion");
    AssetManagerInstance.loadSprite("robot_boss_shoot", "robot_boss_shoot");
    AssetManagerInstance.loadSprite("robot_boss_fordward", "robot_boss_fordward");
    AssetManagerInstance.loadSprite("robot_boss_backward", "robot_boss_backward");
    AssetManagerInstance.loadSprite("directional_canon", "directional_canon");
    AssetManagerInstance.loadSprite("eye_bomber", "eye_bomber");
    AssetManagerInstance.loadSprite("eye_minion", "eye_minion");
    AssetManagerInstance.loadSprite("eye_bigion_normal", "eye_bigion_normal");
    AssetManagerInstance.loadSprite("eye_bigion_angry", "eye_bigion_angry");
    AssetManagerInstance.loadSprite("eye_boss_short_range", "eye_boss_short_range");
    AssetManagerInstance.loadSprite("eye_boss_mid_range", "eye_boss_mid_range");
    AssetManagerInstance.loadSprite("eye_boss_long_range", "eye_boss_long_range");
    AssetManagerInstance.loadSprite("eye_laser", "eye_laser");
    AssetManagerInstance.loadSprite("space_vortex", "space_vortex");
    AssetManagerInstance.loadSprite("space_laser", "space_laser");
    AssetManagerInstance.loadSprite("small_laser", "small_laser");
    AssetManagerInstance.loadSprite("fireball", "fireball");
    AssetManagerInstance.loadSprite("big_bullet", "big_bullet");
    AssetManagerInstance.loadSprite("mid_bullet", "mid_bullet");
    set_sprite_scales();
}

void GameInstance::set_sprite_scales() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.getSprite("spaceship_up")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("spaceship_idle")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("spaceship_down")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("spaceship_other")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("bullet")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("space_plane")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("space_drill")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("space_sphere")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("turret_canon")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("turret_pedestal")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("mob_spawner_ship")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("mortar_rocket")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("robot_boss_minion")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("robot_boss_shoot")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("robot_boss_fordward")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("robot_boss_backward")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("directional_canon")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_bomber")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_minion")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_bigion_normal")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_bigion_angry")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_boss_short_range")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_boss_mid_range")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_boss_long_range")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("eye_laser")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("space_vortex")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("space_laser")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("small_laser")->setScale(1, 1);
    AssetManagerInstance.getSprite("fireball")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("big_bullet")->setScale(GAME_SCALE, GAME_SCALE);
    AssetManagerInstance.getSprite("mid_bullet")->setScale(GAME_SCALE, GAME_SCALE);
}
