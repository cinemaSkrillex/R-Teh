/*
** EPITECH PROJECT, 2024
** game_init
** File description:
** GameInit
*/

#include "Game/Game.hpp"

namespace rtype {

Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _window("SKRILLEX client_port: " + std::to_string(client_port), sf::Vector2u(800, 600)),
      _clock(),
      _controls(_registry, clientUDP),
      _lagCompensationSystem(),
      _movementSystem(),
      _drawSystem(&_window.getRenderWindow()),
      _controlSystem(_window),
      _collisionSystem(),
      _aiSystem(),
      _player_entity(nullptr),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _parallaxSystem(),
      _destructibleSystem(),
      _view(_window.getRenderWindow(), {800 / 2, 600 / 2}, {800, 600}),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    init_all_game();
    // SpaceSphere mob(_registry, {1700, 300}, {0, 0}, 0);
    // EyeMinion   mob_eye(_registry, {1700, 300}, {0, 0}, 0);
    // mob_eye.setTarget(_player_entity, _registry);
    // DirectionalCanon mob(_registry, {1700, 300}, {0, 0}, 0, mob_sprite, true);
    // WallTurret mob(_registry, {1700, 300}, mob_sprite, second_mob_sprite, false);
    // mob.setTarget(_player_entity, _registry);
}

Game::~Game() {}

void Game::init_all_game() {
    init_registry();
    init_controls();
    init_systems();
    init_level("../../assets/sprites/tiles/lv1", "lvl1");
    init_textures();
    set_sprite_opacity();
    init_sprite_sheets();
    init_screen_limits();

    Background background(_registry, -200.f, 3);
    Background background2(_registry, -100.f, 2);
    Background background3(_registry, -50.f, 1);
    _backgroundEntities.push_back(background.getEntity());
    _backgroundEntities.push_back(background2.getEntity());
    _backgroundEntities.push_back(background3.getEntity());
    Player player(_registry, {200, 200}, false);
    _player_entity = player.getEntity();
}

void Game::init_registry() { register_components(); }

void Game::init_controls() {
    bind_keys();
    set_action_handlers();
}

void Game::init_screen_limits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawn_entity();
    _registry.add_component(
        topWall, RealEngine::Collision{
                     {0, -20, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawn_entity();
    _registry.add_component(
        bottomWall, RealEngine::Collision{
                        {0, 600, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawn_entity();
    _registry.add_component(
        leftWall, RealEngine::Collision{
                      {-20, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawn_entity();
    _registry.add_component(
        rightWall, RealEngine::Collision{
                       {800, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
}

void Game::init_textures() {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
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

void Game::init_level(std::string filepath, std::string foldername) {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadTexturesFromFolder(filepath, foldername, {GAME_SCALE, GAME_SCALE});
}

void Game::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
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
        _parallaxSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _particleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });
}

void Game::register_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Interpolation>();
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
    _registry.register_component<RealEngine::Parallax>();
    _registry.register_component<RealEngine::ParticleEmitter>();
    _registry.register_component<RealEngine::Netvar>();
    _registry.register_component<RealEngine::NetvarContainer>();
}

void Game::bind_keys() {
    _controlSystem.bindKey(sf::Keyboard::Z, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::S, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Q, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::D, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Up, RealEngine::Action::Up);
    _controlSystem.bindKey(sf::Keyboard::Down, RealEngine::Action::Down);
    _controlSystem.bindKey(sf::Keyboard::Left, RealEngine::Action::Left);
    _controlSystem.bindKey(sf::Keyboard::Right, RealEngine::Action::Right);
    _controlSystem.bindKey(sf::Keyboard::Space, RealEngine::Action::Action1, true);
}

void Game::set_action_handlers() {
    _controlSystem.setActionHandler(RealEngine::Action::Up,
                                    std::bind(&rtype::Controls::moveUp, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Down,
                                    std::bind(&rtype::Controls::moveDown, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Left,
                                    std::bind(&rtype::Controls::moveLeft, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Right,
                                    std::bind(&rtype::Controls::moveRight, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHandler(RealEngine::Action::Action1,
                                    std::bind(&rtype::Controls::shoot, &_controls,
                                              std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionHoldHandler(RealEngine::Action::Action1,
                                        std::bind(&rtype::Controls::holdShoot, &_controls,
                                                  std::placeholders::_1, std::placeholders::_2));
    _controlSystem.setActionReleaseHandler(RealEngine::Action::Action1,
                                           std::bind(&rtype::Controls::releaseShoot, &_controls,
                                                     std::placeholders::_1, std::placeholders::_2));
}

void Game::set_sprite_opacity() {
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other")->setOpacity(90);
}

void Game::init_sprite_sheets() {
    // load eye minion sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> eyeSheet;
    RealEngine::Sprite                                  eyeSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_minion")));
    eyeSheet.emplace("normal", eyeSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "eye_minion", eyeSheet, "normal", 0, {18, 11}, false, true, 120, {14, 5}, sf::Clock());

    // load space sphere sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> spaceSphereSheet;
    RealEngine::Sprite                                  spaceSphereSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("space_sphere")));
    spaceSphereSheet.emplace("normal", spaceSphereSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("space_sphere", spaceSphereSheet,
                                                            "normal", 0, {16, 14}, false, true, 55,
                                                            {8, 8}, sf::Clock());

    // load mid bullet sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> midBulletSheet;
    RealEngine::Sprite                                  midBulletSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("mid_bullet")));
    midBulletSheet.emplace("normal", midBulletSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "mid_bullet", midBulletSheet, "normal", 0, {16, 12}, false, true, 55, {8, 6}, sf::Clock());

    // load big bullet sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> bigBulletSheet;
    RealEngine::Sprite                                  bigBulletSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("big_bullet")));
    bigBulletSheet.emplace("normal", bigBulletSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "big_bullet", bigBulletSheet, "normal", 0, {32, 12}, false, true, 55, {16, 6}, sf::Clock());
}

}  // namespace rtype