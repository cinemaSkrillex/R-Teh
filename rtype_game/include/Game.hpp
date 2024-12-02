

#include <SFML/Graphics.hpp>

#include "../../game_engine/include/ECS/Components/Acceleration.hpp"
#include "../../game_engine/include/ECS/Components/Controllable.hpp"
#include "../../game_engine/include/ECS/Components/Drawable.hpp"
#include "../../game_engine/include/ECS/Components/Health.hpp"
#include "../../game_engine/include/ECS/Components/Position.hpp"
#include "../../game_engine/include/ECS/Components/Radius.hpp"
#include "../../game_engine/include/ECS/Components/Rotation.hpp"
#include "../../game_engine/include/ECS/Components/Sprite.hpp"
#include "../../game_engine/include/ECS/Components/SpriteSheet.hpp"
#include "../../game_engine/include/ECS/Components/Target.hpp"
#include "../../game_engine/include/ECS/Components/Velocity.hpp"
#include "../../game_engine/include/ECS/Registry/Registry.hpp"
#include "../../game_engine/include/ECS/Systems/AISystem.hpp"
#include "../../game_engine/include/ECS/Systems/CollisionSystem.hpp"
#include "../../game_engine/include/ECS/Systems/ControlSystem.hpp"
#include "../../game_engine/include/ECS/Systems/DrawSystem.hpp"
#include "../../game_engine/include/ECS/Systems/MovementSystem.hpp"
#include "../../game_engine/include/ECS/Systems/RadiusSystem.hpp"
#include "../../game_engine/include/ECS/Systems/RotationSystem.hpp"
#include "../../game_engine/include/ECS/Systems/healthSystem.hpp"
#include "../../game_engine/include/Media/Audio/Music.hpp"
#include "../../game_engine/include/Media/Audio/Sound.hpp"
#include "../../game_engine/include/Media/Graphics/Rendering/Background.hpp"
#include "../../game_engine/include/Media/Graphics/Rendering/Sprite.hpp"
#include "../../game_engine/include/Scripting/LuaManager.hpp"
#include "Client/UDPClient.hpp"
#include "CollisionHandler.hpp"
#include "Controls.hpp"
#include "Core/ClockEngine.hpp"
#include "Core/View.hpp"
#include "Core/Window.hpp"
#include "EyeBoss.hpp"

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
    void               spawn_player();
    void               handleSignal(std::string signal);
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

    std::unique_ptr<EyeBoss>                            _bossEye;
    RealEngine::Sprite                                  _upSpaceship;
    RealEngine::Sprite                                  _idleSpaceship;
    RealEngine::Sprite                                  _downSpaceship;
    RealEngine::Sprite                                  _testSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _spaceshipSheet;
    RealEngine::Sprite                                  _groundSprite;
    RealEngine::Entity                                  _entity1;
    RealEngine::Entity                                  _entity2;
    RealEngine::Entity                                  _groundEntity;
};
}  // namespace rtype