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
      _drawSystem(_serverVision ? &_window->getRenderTexture() : nullptr),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _netvarSystem(),
      _game_map(std::make_shared<ServerMap>(getRegistryRef())) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    init_components();
    init_systems();
    init_textures();
    init_sprite_sheets();
    init_screen_limits();
    std::string path = "../../assets/maps/";
    if (!_game_map) {
        return;
    }
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
    _registry.register_component<RealEngine::Score>();
    _registry.register_component<RealEngine::Interpolation>();
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
    std::string path                 = "../../assets/sprites/r_type/";
    auto&       AssetManagerInstance = RealEngine::AssetManager::getInstance();

    init_level(path + "tiles/lv1", "lvl1");
    init_level(path + "tiles/lv2", "lvl2");

    // player
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_up", path + "spaceship.png",
                                                   {0, 0, 32 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_idle", path + "spaceship.png",
                                                   {0, 15, 32, 15});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_down", path + "spaceship.png",
                                                   {0, 15 * 2, 33 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other", path + "spaceship.png");

    AssetManagerInstance.loadSpriteTextureAndScale("bullet", path + "spaceship_bullet.png");

    // ennemies
    AssetManagerInstance.loadSpriteTextureAndScale("space_plane", path + "enemies/space_plane.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_drill", path + "enemies/space_drill.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_sphere",
                                                   path + "enemies/space_sphere.png");
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_minion",
                                                   path + "enemies/boss_minion.png");
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_shoot",
                                                   path + "enemies/mini_boss.png", {0, 0, 47, 43});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_fordward",
                                                   path + "enemies/mini_boss.png", {0, 43, 56, 54});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_backward",
                                                   path + "enemies/mini_boss.png", {0, 97, 49, 50});
    AssetManagerInstance.loadSpriteTextureAndScale("directional_canon",
                                                   path + "enemies/directional_canon.png");
    AssetManagerInstance.loadSpriteTextureAndScale("mob_spawner_ship",
                                                   path + "enemies/mob_spawner.png");
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bomber", path + "enemies/eye_bomber.png");
    AssetManagerInstance.loadSpriteTextureAndScale("eye_minion", path + "enemies/eye_minion.png");
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_normal", path + "enemies/eye_bigion.png", {0, 0, 23 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_angry", path + "enemies/eye_bigion.png", {0, 16, 21 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_short_range", path + "enemies/the_eye/boss.png", {0, 0, 73 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_mid_range", path + "enemies/the_eye/boss.png", {0, 55, 91 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_long_range", path + "enemies/the_eye/boss.png", {0, 55 * 2, 81 * 3, 55});

    // enemies projectiles
    AssetManagerInstance.loadSpriteTextureAndScale("eye_laser",
                                                   path + "enemies/the_eye/laser_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_vortex",
                                                   path + "enemies/space_vortex.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_laser", path + "enemies/laser_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("small_laser", path + "enemies/laser_shoot.png",
                                                   {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("fireball", path + "enemies/fireball.png");
    AssetManagerInstance.loadSpriteTextureAndScale("big_bullet", path + "big_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("mid_bullet", path + "medium_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("space_missile",
                                                   path + "enemies/space_missile.png");
    AssetManagerInstance.loadSpriteTextureAndScale("simple_shoot",
                                                   path + "enemies/simple_shoot.png");
    AssetManagerInstance.loadSpriteTextureAndScale("mini_boss_projectile",
                                                   path + "enemies/mini_boss_shoot.png");
    // powerups
    AssetManagerInstance.loadSpriteTextureAndScale("shoot_powerup", path + "power_up.png",
                                                   {0, 0, 16 * 4, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("speed_powerup", path + "power_up.png",
                                                   {0, 16, 16 * 3, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("heal_powerup", path + "power_up.png",
                                                   {0, 32, 16 * 5, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    // waiting room zone
    AssetManagerInstance.loadSpriteTextureAndScale("ready_zone", path + "ready_zone.png",
                                                   {0.5, 0.5});
    AssetManagerInstance.getSprite("ready_zone")->setOpacity(200);
}

void GameInstance::init_sprite_sheets() {
    std::unordered_map<std::string, RealEngine::Sprite> midBulletSheet;
    RealEngine::Sprite                                  midBulletSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("mid_bullet")));
    midBulletSheet.emplace("normal", midBulletSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "mid_bullet", midBulletSheet, "normal", 0, {16, 12}, false, true, 55, {8, 6}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> bigBulletSheet;
    RealEngine::Sprite                                  bigBulletSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("big_bullet")));
    bigBulletSheet.emplace("normal", bigBulletSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "big_bullet", bigBulletSheet, "normal", 0, {32, 12}, false, true, 55, {16, 6}, sf::Clock());

    // enemies

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

    std::unordered_map<std::string, RealEngine::Sprite> eyeBigionSheet;
    RealEngine::Sprite                                  _eyeBigionSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_bigion_normal")));
    eyeBigionSheet.emplace("normal", _eyeBigionSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("eye_bigion", eyeBigionSheet, "normal",
                                                            0, {23, 16}, false, true, 120, {17, 8},
                                                            sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> eyeBossSheet;
    RealEngine::Sprite                                  _eyeBossShortSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_short_range")));
    RealEngine::Sprite _eyeBossMidSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_mid_range")));
    RealEngine::Sprite _eyeBossLongSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_boss_long_range")));
    eyeBossSheet.emplace("short", _eyeBossShortSprite);
    eyeBossSheet.emplace("mid", _eyeBossMidSprite);
    eyeBossSheet.emplace("long", _eyeBossLongSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "eye_boss", eyeBossSheet, "short", 0, {73, 55}, false, true, 120, {48, 26}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> mobSpawnerSheet;
    RealEngine::Sprite                                  _mobSpawnerSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("mob_spawner_ship")));
    mobSpawnerSheet.emplace("normal", _mobSpawnerSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("mob_spawner_ship", mobSpawnerSheet,
                                                            "normal", 0, {63, 50}, false, true, 135,
                                                            {32, 25}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> robotBossMinionSheet;
    RealEngine::Sprite                                  _robotBossMinionSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_minion")));
    robotBossMinionSheet.emplace("normal", _robotBossMinionSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "robot_boss_minion", robotBossMinionSheet, "normal", 0, {32, 31}, false, true, 230,
        {-1, -1}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> robotMiniBossSheet;
    RealEngine::Sprite                                  _robotMiniBossShoot(
        *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_shoot")));
    RealEngine::Sprite _robotMiniBossFordward(
        *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_fordward")));
    RealEngine::Sprite _robotMiniBossBackward(
        *(RealEngine::AssetManager::getInstance().getSprite("robot_boss_backward")));
    robotMiniBossSheet.emplace("shoot", _robotMiniBossShoot);
    robotMiniBossSheet.emplace("fordward", _robotMiniBossFordward);
    robotMiniBossSheet.emplace("backwards", _robotMiniBossBackward);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("robot_mini_boss", robotMiniBossSheet,
                                                            "shoot", 0, {47, 43}, false, false, 0,
                                                            {-1, -1}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> spaceSphereSheet;
    RealEngine::Sprite                                  _spaceSphereSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_sphere")));
    spaceSphereSheet.emplace("normal", _spaceSphereSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_sphere", spaceSphereSheet,
                                                            "normal", 0, {16, 14}, false, true, 55,
                                                            {8, 8}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> spaceDrillSheet;
    RealEngine::Sprite                                  _spaceDrillSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_drill")));
    spaceDrillSheet.emplace("normal", _spaceDrillSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_drill", spaceDrillSheet,
                                                            "normal", 0, {32, 22}, true, true, 40,
                                                            {16, 11}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> spacePlaneSheet;
    RealEngine::Sprite                                  _spacePlaneSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_plane")));
    spacePlaneSheet.emplace("normal", _spacePlaneSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_plane", spacePlaneSheet,
                                                            "normal", 0, {21, 23}, false, true, 75,
                                                            {10, 12}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> directionalCanonSheet;
    RealEngine::Sprite                                  _directionalCanonSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("directional_canon")));
    directionalCanonSheet.emplace("normal", _directionalCanonSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "directional_canon", directionalCanonSheet, "normal", 0, {64, 64}, false, true, 80,
        {32, 32}, sf::Clock());

    // mobs projectiles

    std::unordered_map<std::string, RealEngine::Sprite> fireballSheet;
    RealEngine::Sprite                                  _fireballSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("fireball")));
    fireballSheet.emplace("normal", _fireballSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "fireball", fireballSheet, "normal", 0, {12, 12}, false, true, 100, {6, 6}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> miniBossShootSheet;
    RealEngine::Sprite                                  _miniBossShootSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("mini_boss_projectile")));
    miniBossShootSheet.emplace("normal", _miniBossShootSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "mini_boss_projectile", miniBossShootSheet, "normal", 0, {63, 16}, false, true, 55, {50, 8},
        sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> smallLaserSheet;
    RealEngine::Sprite                                  _smallLaserSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("small_laser")));
    smallLaserSheet.emplace("normal", _smallLaserSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("small_laser", smallLaserSheet,
                                                            "normal", 0, {48, 4}, false, true, 55,
                                                            {-1, -1}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> spaceVortexSheet;
    RealEngine::Sprite                                  _spaceVortexSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_vortex")));
    spaceVortexSheet.emplace("normal", _spaceVortexSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_vortex", spaceVortexSheet,
                                                            "normal", 0, {32, 28}, false, true, 90,
                                                            {16, 14}, sf::Clock());
    // powerups
    std::unordered_map<std::string, RealEngine::Sprite> shootPowerUpSheet;
    RealEngine::Sprite                                  shootPowerUpSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("shoot_powerup")));
    shootPowerUpSheet.emplace("normal", shootPowerUpSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("shoot_powerup", shootPowerUpSheet,
                                                            "normal", 0, {16, 16}, false, true, 90,
                                                            {8, 8}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> speedPowerUpSheet;
    RealEngine::Sprite                                  speedPowerUpSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("speed_powerup")));
    speedPowerUpSheet.emplace("normal", speedPowerUpSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("speed_powerup", speedPowerUpSheet,
                                                            "normal", 0, {16, 16}, false, true, 90,
                                                            {8, 8}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> healPowerUpSheet;
    RealEngine::Sprite                                  healPowerUpSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("heal_powerup")));
    healPowerUpSheet.emplace("normal", healPowerUpSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("heal_powerup", healPowerUpSheet,
                                                            "normal", 0, {16, 16}, false, true, 90,
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