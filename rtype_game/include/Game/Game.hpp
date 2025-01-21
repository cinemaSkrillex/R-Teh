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
#include "Game/Background.hpp"
#include "Game/GameMap.hpp"
#include "Game/Particles.hpp"
#include "Game/Player/Player.hpp"
#include "Game/PlayerUI.hpp"
#include "Launcher/LaunchGame.hpp"
#include "Log.hpp"
#include "shared/RtypeServerProtocol.hpp"

namespace rtype {

// Create a structure to hold the entity and its type
struct EntityInfo {
    std::shared_ptr<RealEngine::Entity> entity;
    RTypeProtocol::EntityType           type;
};

#define LaunchGameWithoutArgs false
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP, unsigned short clientPort);
    ~Game();
    void                                run();
    void                                setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    void                                handleSignal(std::array<char, 800> signal);
    std::shared_ptr<RealEngine::Entity> add_player(long int player_uuid, sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> add_mob(long int enemy_uuid, sf::Vector2f position);
    sf::IntRect                         getPlayerNormalizedDirection();
    std::unordered_map<long int, EntityInfo>& getEntities() { return _entities; }
    bool                                      _isInit = false;

    std::shared_ptr<UDPClient> _clientUDP;

   private:
    // initialization functions (src/Game/GameInit.cpp)
    void initAllGame();
    void initRegistry();
    void initControls();
    void initSystems();
    void initTextures();
    void initSpriteSheets();
    void initMusics();
    void initSounds();
    void initFonts();
    void initScreenLimits();

    void initLevel(std::string filepath, std::string foldername);

    void registerComponents();
    void bindKeys();
    void setActionHandlers();
    void setSpriteOpacity();
    void displayTemporaryText(std::string text, sf::Vector2f position, sf::Color color, int size);
    void handleChangingScene(RTypeProtocol::ChangingSceneMessage parsedPacket);
    // GameSignals
    void               handleNewClient(RTypeProtocol::PlayerMoveMessage parsedPacket);
    void               handleSynchronize(RTypeProtocol::SynchronizeMessage parsedPacket);
    void               handlePlayerMove(RTypeProtocol::PlayerMoveMessage parsedPacket);
    void               handleNewEntity(RTypeProtocol::NewEntityMessage parsedPacket);
    void               handleDestroyEntity(RTypeProtocol::DestroyEntityMessage parsedPacket);
    void               handleMapMessage(RTypeProtocol::MapMessage parsedPacket);
    void               handleEntityUpdate(RTypeProtocol::EntityUpdateMessage parsedPacket);
    void               addEntityToGame(RTypeProtocol::NewEntityMessage     parsedPacket,
                                       std::shared_ptr<RealEngine::Entity> newEntity);
    void               handlePlayerValues(RTypeProtocol::PlayerUpdateDataMessage parsedPacket);
    void               unloadLevel(float x, float y);
    void               relocateAllBlocks();
    float              _deltaTime = 0.f;
    RealEngine::View   _view;
    RealEngine::Window _window;
    sf::Clock          _clock;
    sf::Clock          _broadcastClock;
    int                _serverTick;
    PlayerUI           _playerUI;

    RealEngine::Registry              _registry;
    RealEngine::LagCompensationSystem _lagCompensationSystem;
    RealEngine::DrawSystem            _drawSystem;
    RealEngine::ControlSystem         _controlSystem;
    RealEngine::MovementSystem        _movementSystem;
    RealEngine::CollisionSystem       _collisionSystem;
    RealEngine::AISystem              _aiSystem;
    RealEngine::RotationSystem        _rotationSystem;
    RealEngine::RadiusSystem          _radiusSystem;
    RealEngine::DestroySystem         _destroySystem;
    RealEngine::HealthSystem          _healthSystem;
    RealEngine::ParallaxSystem        _parallaxSystem;
    RealEngine::ParticleSystem        _particleSystem;
    RealEngine::NetvarSystem          _netvarSystem;
    rtype::Controls                   _controls;

    std::shared_ptr<RealEngine::Entity>                               _playerEntity;
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _players;
    std::unordered_map<long int, EntityInfo>                          _entities;
    GameMap*                                                          _gameMap;
    std::vector<std::shared_ptr<RealEngine::TemporaryText>>           _temporaryTexts;

    long int                              _localPlayerUUID;
    std::chrono::steady_clock::time_point _startTime;
    long                                  _serverTime;
};
}  // namespace rtype

#endif /* !GAME_HPP_ */