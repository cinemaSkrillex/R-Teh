/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInit
*/

#include "../include/Game/GameInstance.hpp"

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
    rtype::EyeMinion eyeMinion(_registry, {700, 100}, {0, 0}, 0);
    addAndGetEnemy(eyeMinion.getEntity());
    rtype::SpaceSphere spaceSphere(_registry, {100, 300}, {0, 0}, 0);
    addAndGetEnemy(spaceSphere.getEntity());
    // addAndGetSimpleMob({1000, 100}, {-1, 0}, 50 * 2, 50);
    // addAndGetSimpleMob({2050, 100}, {-1, 0}, 50 * 2, 50);
    // addAndGetSimpleMob({2100, 200}, {-1, 0}, 35 * 2, 100);
    // addAndGetSimpleMob({1500, 400}, {-1, 0}, 35 * 2, 100);
    // addAndGetSimpleMob({1200, 100}, {-1, 0}, 40 * 2, 100);
    // addAndGetSimpleMob({1110, 50}, {-1, 0}, 40 * 2, 100);
    // addAndGetSimpleMob({1800, 500}, {-1, 0}, 40 * 2, 100);
    // addAndGetSimpleMob({1500, 250}, {-1, 0}, 40 * 2, 100);
    // addAndGetSimpleMob({1000, 470}, {-1, 0}, 35 * 2, 100);
    // addAndGetSimpleMob({1710, 170}, {-1, 0}, 35 * 2, 100);
    // addAndGetSimpleMob({1900, 490}, {-1, 0}, 20 * 2, 100);
    // addAndGetSimpleMob({1500, 230}, {-1, 0}, 20 * 2, 100);
    // addAndGetSimpleMob({1600, 70}, {-1, 0}, 42 * 2, 100);
    // addAndGetSimpleMob({1100, 200}, {-1, 0}, 42 * 2, 100);
    // addAndGetSimpleMob({1200, 200}, {-1, 0}, 35 * 2, 100);
}

void GameInstance::init_textures() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();

    init_level("../../assets/sprites/tiles/lv1", "lvl1");

    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_up",
                                                   "../../assets/sprites/spaceship.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 0, 32 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_idle",
                                                   "../../assets/sprites/spaceship.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 15, 32, 15});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "spaceship_down", "../../assets/sprites/spaceship.png", {GAME_SCALE, GAME_SCALE},
        {0, 15 * 2, 33 * 2, 15});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "stars_background", "../../assets/sprites/backgrounds/stars.png", {1, 1});
    AssetManagerInstance.getTexture("stars_background")->setRepeated(true);

    AssetManagerInstance.loadSpriteTextureAndScale(
        "bullet", "../../assets/sprites/spaceship_bullet.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_plane", "../../assets/sprites/enemies/space_plane.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_drill", "../../assets/sprites/enemies/space_drill.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_sphere", "../../assets/sprites/enemies/space_sphere.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "turret_canon", "../../assets/sprites/enemies/turret_canon.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "turret_pedestal", "../../assets/sprites/enemies/turret_pedestal.png",
        {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mob_spawner_ship",
                                                   "../../assets/sprites/enemies/mob_spawner.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mortar_rocket",
                                                   "../../assets/sprites/enemies/mortar_rocket.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_minion",
                                                   "../../assets/sprites/enemies/boss_minion.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_shoot",
                                                   "../../assets/sprites/enemies/mini_boss.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 0, 47, 43});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_fordward",
                                                   "../../assets/sprites/enemies/mini_boss.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 43, 56, 54});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_backward",
                                                   "../../assets/sprites/enemies/mini_boss.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 97, 49, 50});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "directional_canon", "../../assets/sprites/enemies/directional_canon.png",
        {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bomber", "../../assets/sprites/enemies/eye_bomber.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_minion", "../../assets/sprites/enemies/eye_minion.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bigion_normal",
                                                   "../../assets/sprites/enemies/eye_bigion.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 0, 23 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bigion_angry",
                                                   "../../assets/sprites/enemies/eye_bigion.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 16, 21 * 2, 16});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_short_range",
                                                   "../../assets/sprites/enemies/the_eye/boss.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 0, 73 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_mid_range",
                                                   "../../assets/sprites/enemies/the_eye/boss.png",
                                                   {GAME_SCALE, GAME_SCALE}, {0, 55, 91 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_long_range", "../../assets/sprites/enemies/the_eye/boss.png",
        {GAME_SCALE, GAME_SCALE}, {0, 55 * 2, 81 * 3, 55});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_laser", "../../assets/sprites/enemies/the_eye/laser_shoot.png",
        {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_vortex", "../../assets/sprites/enemies/space_vortex.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_laser", "../../assets/sprites/enemies/laser_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "small_laser", "../../assets/sprites/enemies/laser_shoot.png", {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "fireball", "../../assets/sprites/enemies/fireball.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "big_bullet", "../../assets/sprites/big_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mid_bullet", "../../assets/sprites/medium_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "spaceship_other", "../../assets/sprites/spaceship.png", {GAME_SCALE, GAME_SCALE});
}

void GameInstance::init_level(std::string filepath, std::string foldername) {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadTexturesFromFolder(filepath, foldername, {GAME_SCALE, GAME_SCALE});
}

void GameInstance::init_sprites() { set_sprite_scales(); }

void GameInstance::set_sprite_scales() {}
