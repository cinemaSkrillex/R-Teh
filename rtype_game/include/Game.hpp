/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

#include <SFML/Graphics.hpp>
#include <chrono>
#include <regex>

#include "Client/UDPClient.hpp"
#include "Controls.hpp"
#include "Engine.hpp"
#include "EyeBomber.hpp"
#include "EyeBoss.hpp"
#include "EyeMinion.hpp"
#include "Log.hpp"
#include "Macros.hpp"
#include "PlayerUtils.hpp"

namespace rtype {
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port);
    ~Game();
    void                run();
    void                setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    RealEngine::Entity  createEntity();
    void                handleSignal(std::string signal);
    RealEngine::Entity* add_player(long int player_uuid, sf::Vector2f position);
    sf::Vector2f        getPlayerNormalizedDirection();
    void add_component_to_entity(RealEngine::Entity entity, RealEngine::Position position);

    std::shared_ptr<UDPClient> _clientUDP;

   private:
    // initialization functions (src/Game/GameInit.cpp)
    void init_all_game();
    void init_registry();
    void init_controls();
    void init_systems();
    void init_sprites();

    void player_collision_handler(RealEngine::CollisionType collisionType,
                                  RealEngine::Registry& registry, RealEngine::Entity collider);
    void player_collide_with_ground();
    void player_take_damage(RealEngine::Entity collider);

    void register_components();
    void bind_keys();
    void set_action_handlers();
    void add_systems();
    void set_sprite_scales();
    void set_sprite_opacity();
    void populate_sprite_sheet();
    //

    // GameSignals
    void handleNewClient(std::unordered_map<std::string, std::string> parsedPacket);
    void handleSynchronize(std::unordered_map<std::string, std::string> parsedPacket);
    void handlePlayerPosition(std::unordered_map<std::string, std::string> parsedPacket);
    //

    float              _deltaTime = 0.f;
    RealEngine::Window _window;
    RealEngine::View   _view;
    sf::Clock          _clock;
    // RealEngine::LuaManager      _luaManager;
    RealEngine::Registry              _registry;
    RealEngine::LagCompensationSystem _lagCompensationSystem;
    RealEngine::DrawSystem            _drawSystem;
    RealEngine::ControlSystem         _controlSystem;
    RealEngine::MovementSystem        _movementSystem;
    RealEngine::CollisionSystem       _collisionSystem;
    RealEngine::AISystem              _aiSystem;
    RealEngine::RotationSystem        _rotationSystem;
    RealEngine::RadiusSystem          _radiusSystem;
    RealEngine::DestructibleSystem    _destructibleSystem;
    RealEngine::HealthSystem          _healthSystem;
    RealEngine::ReappearingSystem     _reappearingSystem;
    rtype::Controls                   _controls;

    std::unordered_map<int, RealEngine::Entity>         _players;
    std::unique_ptr<EyeBoss>                            _bossEye;
    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    RealEngine::Sprite                                  _otherPlayer;
    RealEngine::Sprite                                  _eyeBomberSprite;
    RealEngine::Sprite                                  _eyeMinionSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Sprite                                  _groundSprite;
    RealEngine::Sprite                                  _backgroundSprite;
    RealEngine::Entity                                  _entity2;
    // std::unique_ptr<EyeMinion>                          _eyeMinion;
    std::vector<std::unique_ptr<EyeBomber>> _eyeMinions;
    std::vector<RealEngine::Entity>         _groundBlocksEntities;
    std::vector<RealEngine::Entity>         _backgroundEntities;
    long int                                _localPlayerUUID;
    std::chrono::steady_clock::time_point   _startTime;
    long                                    _serverTime;
};
}  // namespace rtype

#endif /* !GAME_HPP_ */