/*
** EPITECH PROJECT, 2024
** game_init
** File description:
** GameInit
*/

#include "Game/Game.hpp"

namespace rtype {

Game::Game(std::shared_ptr<UDPClient> clientUDP, unsigned short clientPort)
    : _clientUDP(clientUDP),
      _deltaTime(0.f),
      _view(sf::Vector2f(VIEW_WIDTH / 2, VIEW_HEIGHT / 2),
            sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT + 100)),
      _window("SKRILLEX clientPort: " + std::to_string(clientPort),
              sf::Vector2u(VIEW_WIDTH, VIEW_HEIGHT + 100), _view,
              ASSETLAUNCHER ? "assets/shaders/display_options.frag"
                            : "../../assets/shaders/display_options.frag"),
      _clock(),
      _playerUI(_registry, _window.getRenderTexture()),
      _controls(_registry, clientUDP),
      _lagCompensationSystem(),
      _movementSystem(),
      _drawSystem(&_window.getRenderTexture()),
      _controlSystem(_window),
      _collisionSystem(),
      _aiSystem(),
      _playerEntity(nullptr),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _parallaxSystem(),
      _destroySystem(),
      _particleSystem(),
      _netvarSystem(),
      _gameMap(new GameMap(_registry, this)),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initAllGame();
    displayTemporaryText("Welcome to R-Teh", {VIEW_WIDTH / 2, VIEW_HEIGHT / 2}, sf::Color::Yellow,
                         30);
}

Game::~Game() { std::cout << "Destroying game..." << std::endl; }

void Game::initAllGame() {
    initRegistry();
    initControls();
    initSystems();
    std::string path = "../../assets/sprites/r_type/";
    if (ASSETLAUNCHER == true) {
        path = "assets/sprites/r_type/";
    }
    initLevel(path + "tiles/lv1", "lvl1");
    initLevel(path + "tiles/lv2", "lvl2");
    initTextures();
    setSpriteOpacity();
    initSpriteSheets();
    initMusics();
    initSounds();
    initFonts();
    initScreenLimits();

    Player player(_registry, {200, 200}, false);
    _playerEntity = player.getEntity();
    _playerUI.AssignPlayerToUI(_playerEntity);
}

void Game::initRegistry() { registerComponents(); }

void Game::initControls() {
    bindKeys();
    setActionHandlers();
}

void Game::initScreenLimits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawnEntity();
    _registry.addComponent(
        topWall, RealEngine::Collision{
                     {0, -20, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawnEntity();
    _registry.addComponent(
        bottomWall, RealEngine::Collision{
                        {0, 600, 800, 20}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawnEntity();
    _registry.addComponent(
        leftWall, RealEngine::Collision{
                      {-20, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawnEntity();
    _registry.addComponent(
        rightWall, RealEngine::Collision{
                       {800, 0, 20, 600}, "wall", false, RealEngine::CollisionType::BLOCKING});
}

void Game::initTextures() {
    std::string path = "../../assets/sprites/r_type/";
    if (ASSETLAUNCHER == true) {
        path = "assets/sprites/r_type/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_up", path + "spaceship.png",
                                                   {0, 0, 32 * 2, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_idle", path + "spaceship.png",
                                                   {0, 15, 32, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_down", path + "spaceship.png",
                                                   {0, 15 * 2, 33 * 2, 15},
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_up", path + "spaceship.png",
                                                   {0, 0, 32 * 2, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_idle", path + "spaceship.png",
                                                   {0, 15, 32, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_down", path + "spaceship.png",
                                                   {0, 15 * 2, 33 * 2, 15},
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("small_stars_background",
                                                   path + "backgrounds/stars.png", {1, 1});
    AssetManagerInstance.getTexture("small_stars_background")->setRepeated(true);
    AssetManagerInstance.loadSpriteTextureAndScale("medium_stars_background",
                                                   path + "backgrounds/stars.png", {2, 2});
    AssetManagerInstance.getTexture("medium_stars_background")->setRepeated(true);
    AssetManagerInstance.loadSpriteTextureAndScale("big_stars_background",
                                                   path + "backgrounds/stars.png", {3, 3});
    AssetManagerInstance.getTexture("big_stars_background")->setRepeated(true);
    AssetManagerInstance.loadSpriteTextureAndScale("space_base_background",
                                                   path + "backgrounds/space_base.png", {2, 2});
    AssetManagerInstance.getTexture("space_base_background")->setRepeated(true);

    AssetManagerInstance.loadSpriteTextureAndScale("bullet", path + "spaceship_bullet.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_plane", path + "enemies/space_plane.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_drill", path + "enemies/space_drill.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_sphere", path + "enemies/space_sphere.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "turret_canon", path + "enemies/turret_canon.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "turret_pedestal", path + "enemies/turret_pedestal.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mob_spawner_ship", path + "enemies/mob_spawner.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mortar_rocket", path + "enemies/mortar_rocket.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "robot_boss_minion", path + "enemies/boss_minion.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_shoot",
                                                   path + "enemies/mini_boss.png", {0, 0, 47, 43},
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_fordward",
                                                   path + "enemies/mini_boss.png", {0, 43, 56, 54},
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_backward",
                                                   path + "enemies/mini_boss.png", {0, 97, 49, 50},
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "directional_canon", path + "enemies/directional_canon.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mob_spawner_ship", path + "enemies/mob_spawner.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bomber", path + "enemies/eye_bomber.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_minion", path + "enemies/eye_minion.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bigion_normal",
                                                   path + "enemies/eye_bigion.png",
                                                   {0, 0, 23 * 2, 16}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bigion_angry",
                                                   path + "enemies/eye_bigion.png",
                                                   {0, 16, 21 * 2, 16}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_short_range",
                                                   path + "enemies/the_eye/boss.png",
                                                   {0, 0, 73 * 3, 55}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_boss_mid_range",
                                                   path + "enemies/the_eye/boss.png",
                                                   {0, 55, 91 * 3, 55}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_long_range", path + "enemies/the_eye/boss.png", {0, 55 * 2, 81 * 3, 55},
        {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_laser", path + "enemies/the_eye/laser_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_vortex", path + "enemies/space_vortex.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_laser", path + "enemies/laser_shoot.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("small_laser", path + "enemies/laser_shoot.png",
                                                   {1, 1});
    AssetManagerInstance.loadSpriteTextureAndScale("fireball", path + "enemies/fireball.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("big_bullet", path + "big_shoot.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mid_bullet", path + "medium_shoot.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "space_missile", path + "enemies/space_missile.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "simple_shoot", path + "enemies/simple_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "mini_boss_projectile", path + "enemies/mini_boss_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("shoot_powerup", path + "power_up.png",
                                                   {0, 0, 16 * 4, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("speed_powerup", path + "power_up.png",
                                                   {0, 16, 16 * 3, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("heal_powerup", path + "power_up.png",
                                                   {0, 32, 16 * 5, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});

    // waiting room background
    AssetManagerInstance.loadSpriteTextureAndScale(
        "front_line_base", path + "backgrounds/front_line_base.png", {2, 2});
    AssetManagerInstance.getTexture("front_line_base")->setRepeated(true);
    AssetManagerInstance.getSprite("front_line_base")->setOpacity(100);
    // waiting room zone
    AssetManagerInstance.loadSpriteTextureAndScale("ready_zone", path + "ready_zone.png",
                                                   {0.5, 0.5});
    AssetManagerInstance.getSprite("ready_zone")->setOpacity(200);
}

void Game::initLevel(std::string filepath, std::string foldername) {
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadTexturesFromFolder(filepath, foldername, {GAME_SCALE, GAME_SCALE});
}

void Game::initSystems() {
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _lagCompensationSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _controlSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _aiSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _rotationSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _radiusSystem.update(registry);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _healthSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        if (_gameMap && _gameMap->levelRunning()) _parallaxSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destroySystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _particleSystem.update(registry, deltaTime);
    });
    _registry.addSystem<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });
}

void Game::registerComponents() {
    _registry.registerComponent<RealEngine::Position>();
    _registry.registerComponent<RealEngine::Interpolation>();
    _registry.registerComponent<RealEngine::Velocity>();
    _registry.registerComponent<RealEngine::Health>();
    _registry.registerComponent<RealEngine::SpriteComponent>();
    _registry.registerComponent<RealEngine::SpriteSheet>();
    _registry.registerComponent<RealEngine::Drawable>();
    _registry.registerComponent<RealEngine::Collision>();
    _registry.registerComponent<RealEngine::Controllable>();
    _registry.registerComponent<RealEngine::Acceleration>();
    _registry.registerComponent<RealEngine::AI>();
    _registry.registerComponent<RealEngine::Rotation>();
    _registry.registerComponent<RealEngine::Radius>();
    _registry.registerComponent<RealEngine::Target>();
    _registry.registerComponent<RealEngine::AutoDestructible>();
    _registry.registerComponent<RealEngine::Damage>();
    _registry.registerComponent<RealEngine::Parallax>();
    _registry.registerComponent<RealEngine::ParticleEmitter>();
    _registry.registerComponent<RealEngine::Particle>();
    _registry.registerComponent<RealEngine::Netvar>();
    _registry.registerComponent<RealEngine::NetvarContainer>();
    _registry.registerComponent<RealEngine::Score>();
}

void Game::bindKeys() {
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

void Game::setActionHandlers() {
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

void Game::setSpriteOpacity() {
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_up")->setOpacity(90);
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_idle")->setOpacity(90);
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_down")->setOpacity(90);
}

void Game::initSpriteSheets() {
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

    // load player spaceship sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> playerSheet;
    playerSheet.emplace("idle",
                        *(RealEngine::AssetManager::getInstance().getSprite("spaceship_idle")));
    playerSheet.emplace("up", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_up")));
    playerSheet.emplace("down",
                        *(RealEngine::AssetManager::getInstance().getSprite("spaceship_down")));
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "spaceship", playerSheet, "idle", 0, {32, 15}, false, false, 100, {-1, -1}, sf::Clock());

    // load other spaceship sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> otherPlayerSheet;
    otherPlayerSheet.emplace(
        "idle", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_other_idle")));
    otherPlayerSheet.emplace(
        "up", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_other_up")));
    otherPlayerSheet.emplace(
        "down", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_other_down")));
    RealEngine::AssetManager::getInstance().loadSpriteSheet("spaceship_other", otherPlayerSheet,
                                                            "idle", 0, {32, 15}, false, false, 100,
                                                            {-1, -1}, sf::Clock());
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

void Game::initMusics() {
    std::string path = "../../assets/musics/";
    if (ASSETLAUNCHER == true) {
        path = "assets/musics/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadMusic("level_1",
                                   path + "8-Bit_-Skrillex-Bangarang-cover-by-FrankJavCee.ogg");
    AssetManagerInstance.getMusic("level_1")->setLoop(true);
    AssetManagerInstance.getMusic("level_1")->setVolume(30);
    AssetManagerInstance.loadMusic("level_2",
                                   path + "Battle-Against-a-Rising-Star-MOTHER-Encore-OST.ogg");
    AssetManagerInstance.getMusic("level_2")->setLoop(true);
    AssetManagerInstance.getMusic("level_2")->setVolume(55);

    AssetManagerInstance.loadMusic("waiting_room", path + "waiting_room.ogg");
    AssetManagerInstance.getMusic("waiting_room")->setLoop(true);
    AssetManagerInstance.getMusic("waiting_room")->setVolume(30);
}

void Game::initSounds() {
    std::string path = "../../assets/sounds/";
    if (ASSETLAUNCHER == true) {
        path = "assets/sounds/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSound("shoot", path + "laserShoot.wav");
    AssetManagerInstance.loadSound("big_laser_shoot", path + "bigLaserShoot.wav");
    AssetManagerInstance.loadSound("explosion", path + "explosion.wav");
    AssetManagerInstance.loadSound("hit_hurt", path + "hitHurt.wav");
    AssetManagerInstance.loadSound("powerup_shoot", path + "powerup_shoot.wav");
    AssetManagerInstance.loadSound("powerup_speed", path + "powerup_speed.wav");
    AssetManagerInstance.loadSound("powerup_heal", path + "powerup_heal.wav");
    AssetManagerInstance.loadSound("blip_select", path + "blipSelect.wav");
    return;
}

void Game::initFonts() {
    std::string path = "../../assets/fonts/";
    if (ASSETLAUNCHER == true) {
        path = "assets/fonts/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadFont("pixel", path + "Early_GameBoy.ttf");
}

}  // namespace rtype