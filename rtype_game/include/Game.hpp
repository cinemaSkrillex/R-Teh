

#include <SFML/Graphics.hpp>
#include <regex>

#include "../../game_engine/include/Engine.hpp"
#include "Client/UDPClient.hpp"
#include "CollisionHandler.hpp"
#include "Controls.hpp"
#include "EyeBoss.hpp"
#include "Macros.hpp"

namespace rtype {
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP);
    ~Game();
    void               run();
    void               setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    void               init_registry();
    void               init_controls();
    void               init_systems();
    RealEngine::Entity createEntity();
    void               handleSignal(std::string signal);
    void               add_player(int player_port, sf::Vector2f position);
    sf::Vector2f       getPlayerNormalizedDirection();
    void add_component_to_entity(RealEngine::Entity entity, RealEngine::Position position);

    std::shared_ptr<UDPClient> _clientUDP;

   private:
    float                       _deltaTime = 0.f;
    RealEngine::Window          _window;
    RealEngine::View            _view;
    sf::Clock                   _clock;
    RealEngine::LuaManager      _luaManager;
    RealEngine::Registry        _registry;
    RealEngine::DrawSystem      _drawSystem;
    RealEngine::ControlSystem   _controlSystem;
    RealEngine::MovementSystem  _movementSystem;
    RealEngine::CollisionSystem _collisionSystem;
    RealEngine::AISystem        _aiSystem;
    RealEngine::RotationSystem  _rotationSystem;
    RealEngine::RadiusSystem    _radiusSystem;
    rtype::Controls             _controls;

    std::unordered_map<int, RealEngine::Entity>         _players;
    std::unique_ptr<EyeBoss>                            _bossEye;
    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    RealEngine::Sprite                                  _otherPlayer;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Sprite                                  _groundSprite;
    RealEngine::Entity                                  _entity2;
    std::vector<RealEngine::Entity>                     _groundBlocksEntities;
};
}  // namespace rtype