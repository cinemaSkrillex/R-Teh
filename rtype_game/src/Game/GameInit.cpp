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
      _view(sf::Vector2f(VIEW_WIDTH / 2, VIEW_HEIGHT / 2),
            sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT + 100)),
      _window("SKRILLEX client_port: " + std::to_string(client_port),
              sf::Vector2u(VIEW_WIDTH, VIEW_HEIGHT + 100), _view,
              assetLauncher ? "assets/shaders/display_options.frag"
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
      _player_entity(nullptr),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _parallaxSystem(),
      _destroySystem(),
      _particleSystem(),
      _netvarSystem(),
      _game_map(_registry),
      _localPlayerUUID(0),
      _startTime(std::chrono::steady_clock::now()) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    init_all_game();
    display_temporary_text("Welcome to R-Teh", {VIEW_WIDTH / 2, VIEW_HEIGHT / 2}, sf::Color::Yellow,
                           30);
}

Game::~Game() {}

void Game::init_all_game() {
    init_registry();
    init_controls();
    init_systems();
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    init_level(path + "tiles/lv1", "lvl1");
    init_level(path + "tiles/lv2", "lvl2");
    init_textures();
    set_sprite_opacity();
    init_sprite_sheets();
    init_musics();
    init_sounds();
    init_fonts();
    init_screen_limits();

    Player player(_registry, {200, 200}, false);
    _player_entity = player.getEntity();
    _playerUI.AssignPlayerToUI(_player_entity);
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
    std::string path = "../../assets/sprites/r_type/";
    if (assetLauncher == true) {
        path = "assets/sprites/r_type/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_up", path + "spaceship.png",
                                                   {0, 0, 32 * 2, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_idle", path + "spaceship.png",
                                                   {0, 15, 32, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_down", path + "spaceship.png",
                                                   {0, 15 * 2, 33 * 2, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_up", path + "spaceship.png",
                                                   {0, 0, 32 * 2, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_idle", path + "spaceship.png",
                                                   {0, 15, 32, 15}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("spaceship_other_down", path + "spaceship.png",
                                                   {0, 15 * 2, 33 * 2, 15}, {GAME_SCALE, GAME_SCALE});
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

    AssetManagerInstance.loadSpriteTextureAndScale("bullet", path + "spaceship_bullet.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_plane", path + "enemies/space_plane.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_drill", path + "enemies/space_drill.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_sphere",
                                                   path + "enemies/space_sphere.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("turret_canon",
                                                   path + "enemies/turret_canon.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("turret_pedestal",
                                                   path + "enemies/turret_pedestal.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mob_spawner_ship",
                                                   path + "enemies/mob_spawner.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mortar_rocket",
                                                   path + "enemies/mortar_rocket.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_minion",
                                                   path + "enemies/boss_minion.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_shoot",
                                                   path + "enemies/mini_boss.png", {0, 0, 47, 43}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_fordward",
                                                   path + "enemies/mini_boss.png", {0, 43, 56, 54}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("robot_boss_backward",
                                                   path + "enemies/mini_boss.png", {0, 97, 49, 50}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("directional_canon",
                                                   path + "enemies/directional_canon.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_bomber", path + "enemies/eye_bomber.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_minion", path + "enemies/eye_minion.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_normal", path + "enemies/eye_bigion.png", {0, 0, 23 * 2, 16}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_bigion_angry", path + "enemies/eye_bigion.png", {0, 16, 21 * 2, 16}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_short_range", path + "enemies/the_eye/boss.png", {0, 0, 73 * 3, 55}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_mid_range", path + "enemies/the_eye/boss.png", {0, 55, 91 * 3, 55}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale(
        "eye_boss_long_range", path + "enemies/the_eye/boss.png", {0, 55 * 2, 81 * 3, 55}, {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("eye_laser",
                                                   path + "enemies/the_eye/laser_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_vortex",
                                                   path + "enemies/space_vortex.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("space_laser", path + "enemies/laser_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("small_laser", path + "enemies/laser_shoot.png",
                                                   {GAME_SCALE - 2, GAME_SCALE - 2});
    AssetManagerInstance.loadSpriteTextureAndScale("fireball", path + "enemies/fireball.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("big_bullet", path + "big_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("mid_bullet", path + "medium_shoot.png", {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("shoot_powerup", path + "power_up.png",
                                                   {0, 0, 16 * 4, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("speed_powerup", path + "power_up.png",
                                                   {0, 16, 16 * 3, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
    AssetManagerInstance.loadSpriteTextureAndScale("heal_powerup", path + "power_up.png",
                                                   {0, 32, 16 * 5, 16},
                                                   {GAME_SCALE - 1, GAME_SCALE - 1});
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
        if (_game_map.levelRunning()) _parallaxSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destroySystem.update(registry, deltaTime);
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
    _registry.register_component<RealEngine::Particle>();
    _registry.register_component<RealEngine::Netvar>();
    _registry.register_component<RealEngine::NetvarContainer>();
    _registry.register_component<RealEngine::Score>();
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
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_up")->setOpacity(90);
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_idle")->setOpacity(90);
    RealEngine::AssetManager::getInstance().getSprite("spaceship_other_down")->setOpacity(90);
}

void Game::init_sprite_sheets() {
    // load eye minion sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> eyeSheet;
    RealEngine::Sprite                                  eyeSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_minion")));
    eyeSheet.emplace("normal", eyeSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet(
        "eye_minion", eyeSheet, "normal", 0, {18, 11}, false, true, 120, {14, 5}, sf::Clock());

    std::unordered_map<std::string, RealEngine::Sprite> eyeBomberSheet;
    RealEngine::Sprite                                  _eyeBomberSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("eye_bomber")));
    eyeBomberSheet.emplace("normal", _eyeBomberSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("eye_bomber", eyeBomberSheet, "normal",
                                                            0, {15, 10}, false, true, 120, {11, 5},
                                                            sf::Clock());

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

    // load smal laser sprite sheet
    std::unordered_map<std::string, RealEngine::Sprite> smallLaserSheet;
    RealEngine::Sprite                                  smallLaserSprite(
        *(RealEngine::AssetManager::getInstance().getSprite("small_laser")));
    smallLaserSheet.emplace("normal", smallLaserSprite);
    RealEngine::AssetManager::getInstance().loadSpriteSheet("small_laser", smallLaserSheet,
                                                            "normal", 0, {48, 4}, false, true, 55,
                                                            {-1, -1}, sf::Clock());
}

void Game::init_musics() {
    std::string path = "../../assets/musics/";
    if (assetLauncher == true) {
        path = "assets/musics/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadMusic("level_1",
                                   path + "8-Bit_-Skrillex-Bangarang-cover-by-FrankJavCee.ogg");
    AssetManagerInstance.getMusic("level_1")->setLoop(true);
    AssetManagerInstance.getMusic("level_1")->setVolume(50);
    AssetManagerInstance.loadMusic("level_2",
                                   path + "Battle-Against-a-Rising-Star-MOTHER-Encore-OST.ogg");
    AssetManagerInstance.getMusic("level_2")->setLoop(true);
    AssetManagerInstance.getMusic("level_2")->setVolume(55);
}

void Game::init_sounds() {
    std::string path = "../../assets/sounds/";
    if (assetLauncher == true) {
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

void Game::init_fonts() {
    std::string path = "../../assets/fonts/";
    if (assetLauncher == true) {
        path = "assets/fonts/";
    }
    auto& AssetManagerInstance = RealEngine::AssetManager::getInstance();
    AssetManagerInstance.loadFont("pixel", path + "Early_GameBoy.ttf");
}

}  // namespace rtype