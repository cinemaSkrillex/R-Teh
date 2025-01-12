/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInit
*/

#include "../../include/Game/GameInstance.hpp"

GameInstance::GameInstance(bool serverVision)
    : _serverVision(serverVision),
      _clock(),
      _window(_serverVision ? std::make_unique<RealEngine::Window>("I CAN SEE EVERYTHING",
                                                                   sf::Vector2u(800, 600))
                            : nullptr),
      _registry(),
      _drawSystem(_serverVision ? &_window->getRenderWindow() : nullptr),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _netvarSystem(),
      _game_map(std::make_shared<GameMap>(getRegistryRef())) {
    init_components();
    init_systems();
    init_textures();
    init_sprite_sheets();
    init_screen_limits();
    _game_map->loadFromJSON("../../assets/maps/map.json");
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
    _registry.register_component<RealEngine::Particle>();
    _registry.register_component<RealEngine::ParticleEmitter>();
    _registry.register_component<RealEngine::TargetRadius>();
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
        _destroySystem.update(registry, deltaTime);
    });
}

void GameInstance::init_textures() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();

    init_level("../../assets/sprites/tiles/lv1", "lvl1");

    AssetManagerInstance.loadSpriteTextureAndScale(
        "spaceship_up", "../../assets/sprites/spaceship.png", {0, 0, 32 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "spaceship_idle", "../../assets/sprites/spaceship.png", {0, 15, 32, 15});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "spaceship_down", "../../assets/sprites/spaceship.png", {0, 15 * 2, 33 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "stars_background", "../../assets/sprites/backgrounds/stars.png", {1, 1});
    AssetManagerInstance.getTexture("stars_background")->setRepeated(true);

    AssetManagerInstance.loadSpriteTextureAndScale("bullet",
                                                   "../../assets/sprites/spaceship_bullet.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_plane",
                                                   "../../assets/sprites/enemies/space_plane.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_drill",
                                                   "../../assets/sprites/enemies/space_drill.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_sphere",
                                                   "../../assets/sprites/enemies/space_sphere.png");
    AssetManagerInstance.loadSpriteTextureAndScale("turret_canon",
                                                   "../../assets/sprites/enemies/turret_canon.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "turret_pedestal", "../../assets/sprites/enemies/turret_pedestal.png");
    AssetManagerInstance.loadSpriteTextureAndScale("mob_spawner_ship",
                                                   "../../assets/sprites/enemies/mob_spawner.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mortar_rocket", "../../assets/sprites/enemies/mortar_rocket.png");
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_minion",
                                                   "../../assets/sprites/enemies/boss_minion.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "robot_boss_shoot", "../../assets/sprites/enemies/mini_boss.png", {0, 0, 47, 43});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "robot_boss_fordward", "../../assets/sprites/enemies/mini_boss.png", {0, 43, 56, 54});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "robot_boss_backward", "../../assets/sprites/enemies/mini_boss.png", {0, 97, 49, 50});
    AssetManagerInstance.loadSpriteTextureAndScale("mortar_enemy",
                                                   "../../assets/sprites/enemies/mortar_enemy.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "directional_canon", "../../assets/sprites/enemies/directional_canon.png");
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bomber",
                                                   "../../assets/sprites/enemies/eye_bomber.png");
    AssetManagerInstance.loadSpriteTextureAndScale("eye_minion",
                                                   "../../assets/sprites/enemies/eye_minion.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_normal", "../../assets/sprites/enemies/eye_bigion.png", {0, 0, 23 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_angry", "../../assets/sprites/enemies/eye_bigion.png", {0, 16, 21 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_short_range",
                                                   "../../assets/sprites/enemies/the_eye/boss.png",
                                                   {0, 0, 73 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_mid_range", "../../assets/sprites/enemies/the_eye/boss.png", {0, 55, 91 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_long_range",
                                                   "../../assets/sprites/enemies/the_eye/boss.png",
                                                   {0, 55 * 2, 81 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_laser", "../../assets/sprites/enemies/the_eye/laser_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_vortex",
                                                   "../../assets/sprites/enemies/space_vortex.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_laser",
                                                   "../../assets/sprites/enemies/laser_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "small_laser", "../../assets/sprites/enemies/laser_shoot.png", {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("fireball",
                                                   "../../assets/sprites/enemies/fireball.png");
    AssetManagerInstance.loadSpriteTextureAndScale("big_bullet",
                                                   "../../assets/sprites/big_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("mid_bullet",
                                                   "../../assets/sprites/medium_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other",
                                                   "../../assets/sprites/spaceship.png");
}

void GameInstance::init_sprite_sheets() {
    // load eye minion sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> eyeSheet;
    RealEngine::Sprite _sprite(*(RealEngine::AssetManager::getInstance().getSprite("eye_minion")));
    eyeSheet.emplace("normal", _sprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "eye_minion", eyeSheet, "normal", 0, {18, 11}, false, true, 120, {14, 5}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> eyeBomberSheet;
    RealEngine::Sprite                                  _eyeBomberSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_bomber")));
    eyeBomberSheet.emplace("normal", _eyeBomberSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("eye_bomber", eyeBomberSheet, "normal",
                                                            0, {15, 10}, false, true, 120, {11, 5},
                                                            sf::Clock());
    // //load space sphere sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> spaceSphereSheet;
    RealEngine::Sprite                                  _spaceSphereSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_sphere")));
    spaceSphereSheet.emplace("normal", _spaceSphereSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_sphere", spaceSphereSheet,
                                                            "normal", 0, {16, 14}, false, true, 55,
                                                            {8, 8}, sf::Clock());
}

void GameInstance::init_level(std::string filepath, std::string foldername) {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadTexturesFromFolder(filepath, foldername, {GAME_SCALE, GAME_SCALE});
}

void GameInstance::init_screen_limits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawn_entity();
    _registry.add_component(
        topWall,
        RealEngine::Collision{
            {0, -20, VIEW_WIDTH, 20}, "limits", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawn_entity();
    _registry.add_component(bottomWall, RealEngine::Collision{{0, VIEW_HEIGHT, VIEW_WIDTH, 20},
                                                              "limits",
                                                              false,
                                                              RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawn_entity();
    _registry.add_component(
        leftWall,
        RealEngine::Collision{
            {-20, 0, 20, VIEW_HEIGHT}, "limits", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawn_entity();
    _registry.add_component(rightWall, RealEngine::Collision{{VIEW_WIDTH, 0, 20, VIEW_HEIGHT},
                                                             "limits",
                                                             false,
                                                             RealEngine::CollisionType::BLOCKING});

    std::shared_ptr<RealEngine::Entity> entitiesRect = _registry.spawn_entity();
    _registry.add_component(entitiesRect,
                            RealEngine::Collision{{-100, -50, VIEW_WIDTH + 200, VIEW_HEIGHT + 100},
                                                  "limits",
                                                  false,
                                                  RealEngine::CollisionType::SCREEN});
}